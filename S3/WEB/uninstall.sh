#/bin/sh
# Note that this script must be ran as root user, thus do this before: sudo su -
echo "==="
echo "Deleting wordpress database and user..."
echo "==="
/usr/bin/mariadb -u root -e "DROP DATABASE wordpress"
/usr/bin/mariadb -u root -e "DROP USER 'wordpress'@'localhost'"

echo "==="
echo "Removing dependencies..."
echo "==="
dpkg --configure -a
apt -y purge lynx
apt -y purge apache2
apt -y purge mariadb-server 
apt -y purge mariadb-client
apt -y purge php php-bz2 php-mysqli php-curl php-gd php-intl php-common php-mbstring php-xml
apt autoremove # Removes not needed dependencies

echo "==="
echo "Uninstalling WordPress..."
echo "==="
rm -r /var/www/html

echo "==="
echo "Finished!"
echo "==="
