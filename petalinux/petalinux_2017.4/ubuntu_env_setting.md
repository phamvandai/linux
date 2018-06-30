# install MS Font
sudo apt-get install ttf-mscorefonts-installer

# install custom fonts
sudo apt-get install fontmanager 
copy font to .fonts folder

# install classic menu
sudo apt-add-repository ppa:diesch/testing
sudo apt-get update
sudo apt-get install classicmenu-indicator

# install ibus-unikey
sudo apt-get install ibus-unikey

# install gdebi package manager 
sudo apt-get install gdebi

# install 4pane file manager
goto 4pane.co.uk download .deb and install using gdebi

# install dev tools
cmake
vscode

# fix time difference between ubuntu & windows
timedatectl set-local-rtc 1 --adjust-system-clock

# install unrar
sudo apt-get install unrar

# install clang 

# install lcov (coverage test)

# install dhcp server
sudo apt-get install isc-dhcp-server

sudo vi /etc/default/isc-dhcp-server
assign INTERFACES = "enp3s0" -> inteface name 

sudo vi /etc/dhcp/dhcpd.conf

# Colorized terminal
  Open .bashrc and uncomment force_color_prompt=yes
  Save and source bashrc
