#/bin/sh
# Note that this script must be ran as root user, thus do this before: sudo su -
echo "==="
echo "Removing dependencies..."
echo "==="
apt -y remove lynx
apt -y remove apache2
apt -y remove mariadb-server 
apt -y remove mariadb-client
apt -y remove php php-bz2 php-mysqli php-curl php-gd php-intl php-common php-mbstring php-xml
echo "==="
echo "Uninstalling WordPress..."
echo "==="
rm -r /var/www/html
echo "==="
echo "Finished!"
echo "==="
