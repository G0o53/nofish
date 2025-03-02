#!/bin/sh

gcc -o nfish nfish.c
cp nfish nofish

# Define the location where the binaries will be installed
INSTALL_DIR="/usr/local/bin"

# Check if the script is being run as root (required for writing to /usr/local/bin)
if [ "$(id -u)" -ne 0 ]; then
    echo "You must run this script as root (using sudo)"
    exit 1
fi

# Check if the binaries already exist at the destination
if [ -f "$INSTALL_DIR/nfish" ] || [ -f "$INSTALL_DIR/nofish" ]; then
    echo "Either 'nfish' or 'nofish' is already installed at $INSTALL_DIR."
    exit 1
fi

# Install the nofish binary as both 'nofish' and 'nfish'
echo "Installing nofish as both 'nofish' and 'nfish' in $INSTALL_DIR"

# Copy the binary to both nfish and nofish
cp ./nofish "$INSTALL_DIR/nfish"
cp ./nofish "$INSTALL_DIR/nofish"

# Check if the installation was successful
if [ $? -eq 0 ]; then
    echo "nofish successfully installed as both 'nfish' and 'nofish'."
else
    echo "There was an error installing nofish."
    exit 1
fi

# Make sure the installed shells are executable
chmod +x "$INSTALL_DIR/nfish"
chmod +x "$INSTALL_DIR/nofish"

# Add nofish to the user's PATH (optional, depending on the use case)
# This step is optional, as it's typically handled by having /usr/local/bin in the PATH already
# Uncomment the following line if you need to add the binary to PATH explicitly
# echo "export PATH=\$PATH:$INSTALL_DIR" >> ~/.bashrc

# Finish
echo "Installation complete. You can now run nofish as either 'nfish' or 'nofish' from anywhere."

exit 0
