## 1. Ubuntu server installation
1. Install [VirtualBox](https://www.virtualbox.org/wiki/Downloads) on your machine (host).
2. Download Ubuntu server .iso file from [here](https://ubuntu.com/download/server).
3. Open VirtualBox and create a virtual machine (VM/guest) from the downloaded .iso file.
4. Go into the VM network settings, open the dropdown menu, set the connection to `NAT` and click on port forwarding.
5. If you want to connect via SSH, make sure you have a rule that has the host port set to 3022 and guest port set to 22 (the other fields can stay blank).
6. Also add a rule that has the host port set to 8080 and guest port set to 80 (the other fields can stay blank).
7. If the VM has issues connecting to the internet (test with `ping -c 5 google.com`), close it
and execute `VBoxManage modifyvm "<VM name>" --natdnshostresolver1 on` in your host terminal (on Windows navigate to "C:\Program Files\Oracle\VirtualBox" before).
This command forces the VM to use the hosts DNS resolver instead of its own.


## 2. WordPress + CiviCRM installation
1. Connect to the VM `<username>@localhost:3022` over your favorite SSH terminal emulator.
2. Switch to root user with:
```sh
sudo su -
```
3. Download and execute the installer script with: 
```sh
curl -LsS https://raw.githubusercontent.com/Arman-Kandel/HS-Niederrhein-Informatik-Bachelor/main/S3/WEB/install.sh | bash -s --
```
4. (host) Open your browser and navigate to `localhost` to complete the WordPress setup.
5. (host) Open the plugins tab and activate the CiviCRM plugin.
6. To uninstall execute:
```sh
curl -LsS https://raw.githubusercontent.com/Arman-Kandel/HS-Niederrhein-Informatik-Bachelor/main/S3/WEB/uninstall.sh | bash -s --
```

## 3. Install extension
This extension displays all households. There are multiple filters available.
1. Download the extension .zip file from [here](TODO).
2. Unpack it into `/var/www/html/wp-content/plugins/civicrm/civicrm/ext`
3. Inside the CiviCRM GUI go to `Administer -> System Settings -> Extensions` (or refresh if already there).
4. Find the extension and press on install.
5. Currently available under page: `http://<enter-url>/wp-admin/admin.php?page=CiviCRM&q=civicrm%2Fmy-page`

## Developers
To work on the extension fork it from [here](TODO). Its recommended to use civix, to install execute these commands:
```sh
sudo curl -LsS https://download.civicrm.org/civix/civix.phar -o /usr/local/bin/civix
sudo chmod +x /usr/local/bin/civix
civix list # shows all commnands
```
