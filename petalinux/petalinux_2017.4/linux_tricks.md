# use high baudrate tty on linux
screen /dev/ttyUSB0 460800

# list screen session
screen -ls

# properly quit screen
Ctrl + A then k
Ctrl + A then d -> detach

screen -X -S sesion_id quit 

# Restore to screen session
screen -r sesion_id