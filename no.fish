#!/bin/sh

gcc -o nfish nfish.c
cp nfish nofish

INSTALL_DIR="/usr/local/bin"

if [ "$(id -u)" -ne 0 ]; then
    echo "You must run this script as root (using sudo)"
    exit 1
fi

if [ -f "$INSTALL_DIR/nfish" ] || [ -f "$INSTALL_DIR/nofish" ]; then
    echo "Either 'nfish' or 'nofish' is already installed at $INSTALL_DIR."
    exit 1
fi

echo "Installing nofish as both 'nofish' and 'nfish' in $INSTALL_DIR"

cp ./nofish "$INSTALL_DIR/nfish"
cp ./nofish "$INSTALL_DIR/nofish"

if [ $? -eq 0 ]; then
    echo "nofish successfully installed as both 'nfish' and 'nofish'."
else
    echo "There was an error installing nofish."
    exit 1
fi

chmod +x "$INSTALL_DIR/nfish"
chmod +x "$INSTALL_DIR/nofish"

echo "Installation complete. You can now run nofish as either 'nfish' or 'nofish' from anywhere."

exit 0
