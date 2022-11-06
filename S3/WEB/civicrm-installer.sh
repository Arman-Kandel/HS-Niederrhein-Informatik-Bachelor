#/bin/sh
# Get CiviCRM
apt -y install zip
wget https://download.civicrm.org/civicrm-5.55.0-wordpress.zip
unzip civicrm-5.55.0-wordpress.zip -d /var/www/civicrm/wp-content/plugins/

# Configure Apache
cat << EOF >> /etc/apache2/sites-available/civicrm.conf
<VirtualHost *:80>

    ServerAdmin admin@example.com

    ServerName example.com

    DocumentRoot /var/www/civicrm



    <Directory /var/www/civicrm>

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
