#!/bin/sh

# This script will uninstall both 'nfish' and 'nofish' from /usr/local/bin

# Define the location where the binaries are installed
INSTALL_DIR="/usr/local/bin"

# Check if the script is being run as root (required for writing to /usr/local/bin)
if [ "$(id -u)" -ne 0 ]; then
    echo "You must run this script as root (using sudo)"
    exit 1
fi

# Check if the binaries exist before attempting to remove them
if [ ! -f "$INSTALL_DIR/nfish" ] && [ ! -f "$INSTALL_DIR/nofish" ]; then
    echo "Neither 'nfish' nor 'nofish' is installed in $INSTALL_DIR."
    exit 1
fi

# Remove the binaries
echo "Uninstalling 'nfish' and 'nofish' from $INSTALL_DIR"

# Remove the 'nfish' and 'nofish' binaries
rm -f "$INSTALL_DIR/nfish"
rm -f "$INSTALL_DIR/nofish"

# Check if the removal was successful
if [ $? -eq 0 ]; then
    echo "'nfish' and 'nofish' have been successfully uninstalled."
else
    echo "There was an error during the uninstallation process."
    exit 1
fi

# Finish
echo "Uninstallation complete."

exit 0

