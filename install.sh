#!/bin/bash

# Create a directory named "build" if it doesn't already exist
if [ ! -d "build" ]; then
  mkdir -p build
  if [ $? -ne 0 ]; then
    echo "Error: Failed to create the 'build' directory."
    exit 1
  fi
fi

# Change to the "build" directory
# shellcheck disable=SC2164
cd build
if [ $? -ne 0 ]; then
  echo "Error: Failed to change to the 'build' directory."
  exit 1
fi

# Run CMake to configure the project (assuming you have a CMakeLists.txt file in the parent directory)
cmake ..
if [ $? -ne 0 ]; then
  echo "Error: CMake configuration failed."
  exit 1
fi

# Compile and install the project (use 'sudo' only if necessary)
make
if [ $? -ne 0 ]; then
  echo "Error: Compilation failed."
  exit 1
fi

# Install the project (use 'sudo' only if necessary)
sudo make install
if [ $? -ne 0 ]; then
  echo "Error: Installation failed."
  exit 1
fi

# Change back to the previous directory (optional, depends on your needs)
# shellcheck disable=SC2103
cd ..

# Run the 'scot' command with the '--help' option to get help
scot --help

# If all steps succeeded, provide a success message
echo "All steps completed successfully."
