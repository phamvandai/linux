# Ubuntu 16.04 needed packages

## Fonts
ttf-mscorefonts-installer
font-manager 
copy font to .fonts folder

## Classic menu
sudo apt-add-repository ppa:diesch/testing
sudo apt-get update
sudo apt-get install classicmenu-indicator

## ibus-unikey
ibus-unikey
ibus restart

## Package manager 
gdebi
synaptic

## Dev tools
cmake
vscode
git
clang
lcov
nload
build-essential

## fix time difference between ubuntu & windows
timedatectl set-local-rtc 1 --adjust-system-clock

## unrar
unrar

## psensor
psensor

# Colorized terminal
  Open .bashrc and uncomment force_color_prompt=yes
  Save and source bashrc
