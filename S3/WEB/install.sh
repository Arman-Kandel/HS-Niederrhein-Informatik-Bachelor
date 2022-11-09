#/bin/sh
# Note that this script must be ran as root user, thus do this before: sudo su -
# Source: https://www.how2shout.com/linux/script-to-install-lamp-wordpress-on-ubuntu-20-04-lts-server-quickly-with-one-command/
# Source: https://www.linuxcapable.com/how-to-install-wordpress-with-lamp-stack-on-debian-11-bullseye/

echo "==="
echo "Creating credentials..."
echo "==="
install_dir="/var/www/html"
db_name="wordpress"
db_user="wordpress"
db_password=`date |md5sum |cut -c '1-12'`
sleep 1

echo "==="
echo "Installing and enabling dependencies..."
echo "==="
apt -y update 
apt -y upgrade
apt -y install zip
apt -y install lynx
apt -y install apache2
curl -LsS https://downloads.mariadb.com/MariaDB/mariadb_repo_setup | bash -s -- --mariadb-server-version=10.7 --skip-maxscale --skip-tools
apt -y install mariadb-server mariadb-client


#### Start http
rm /var/www/html/index.html
systemctl enable apache2
systemctl start apache2

#### Start mariadb
systemctl enable mariadb
systemctl start mariadb

####Install PHP
apt -y install php php-bz2 php-mysqli php-curl php-gd php-intl php-common php-mbstring php-xml php-zip

sed -i '0,/AllowOverride\ None/! {0,/AllowOverride\ None/ s/AllowOverride\ None/AllowOverride\ All/}' /etc/apache2/apache2.conf #Allow htaccess usage

systemctl restart apache2

####Download and extract latest WordPress Package
if test -f /tmp/latest.tar.gz
then
echo "==="
echo "WordPress is already downloaded."
echo "==="
else
echo "==="
echo "Downloading WordPress..."
echo "==="
cd /tmp/ && wget "http://wordpress.org/latest.tar.gz";
fi

echo "==="
echo "Installing WordPress..."
echo "==="
/bin/tar -C $install_dir -zxf /tmp/latest.tar.gz --strip-components=1
chown www-data: $install_dir -R

#### Create WP-config and set DB credentials
/bin/mv $install_dir/wp-config-sample.php $install_dir/wp-config.php

/bin/sed -i "s/database_name_here/$db_name/g" $install_dir/wp-config.php
/bin/sed -i "s/username_here/$db_user/g" $install_dir/wp-config.php
/bin/sed -i "s/password_here/$db_password/g" $install_dir/wp-config.php

cat << EOF >> $install_dir/wp-config.php
define('FS_METHOD', 'direct');
EOF

cat << EOF >> $install_dir/.htaccess
# BEGIN WordPress
<IfModule mod_rewrite.c>
RewriteEngine On
RewriteBase /
RewriteRule ^index.php$ – [L]
RewriteCond %{REQUEST_FILENAME} !-f
RewriteCond %{REQUEST_FILENAME} !-d
RewriteRule . /index.php [L]
</IfModule>
# END WordPress
EOF

chown www-data: $install_dir -R

##### Set WP Salts
grep -A50 'table_prefix' $install_dir/wp-config.php > /tmp/wp-tmp-config
/bin/sed -i '/**#@/,/$p/d' $install_dir/wp-config.php
/usr/bin/lynx --dump -width 200 https://api.wordpress.org/secret-key/1.1/salt/ >> $install_dir/wp-config.php
/bin/cat /tmp/wp-tmp-config >> $install_dir/wp-config.php && rm /tmp/wp-tmp-config -f
/usr/bin/mariadb -u root -e "CREATE DATABASE $db_name"
/usr/bin/mariadb -u root -e "CREATE USER '$db_name'@'localhost' IDENTIFIED BY '$db_password';"
/usr/bin/mariadb -u root -e "GRANT ALL PRIVILEGES ON $db_name.* TO '$db_user'@'localhost' IDENTIFIED BY '$db_password';"
/usr/bin/mariadb -u root -e "FLUSH PRIVILEGES;"

echo "==="
echo "Installing CiviCRM..."
echo "==="
# Get CiviCRM
wget -O civicrm.zip https://download.civicrm.org/civicrm-5.55.0-wordpress.zip
unzip civicrm.zip -d /var/www/html/wp-content/plugins/

# Configure Apache
cat << EOF >> /etc/apache2/sites-available/civicrm.conf
<VirtualHost *:80>
    ServerAdmin admin@example.com
    ServerName example.com
    DocumentRoot /var/www/html
    <Directory /var/www/html>
        Options Indexes FollowSymLinks
        AllowOverride All
        Require all granted
    </Directory>
    ErrorLog ${APACHE_LOG_DIR}/error.log
    CustomLog ${APACHE_LOG_DIR}/access.log combined
</VirtualHost>
EOF

a2ensite /etc/apache2/sites-available/civicrm.conf
a2dissite /etc/apache2/sites-available/000-default.conf
a2enmod rewrite
systemctl restart apache2

echo "==="
echo "Finished! Save the generated credentials somewhere."
echo "==="
######Display generated passwords to log file.
echo "WordPress available at: http://localhost"
echo "Database Name: " $db_name
echo "Database User: " $db_user
echo "Database Password: " $db_password
