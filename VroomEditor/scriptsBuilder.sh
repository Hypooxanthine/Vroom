#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ]; then
  echo "Error : two arguments are needed."
  echo "Usage : $0 <cmake_path> <build_path>"
  exit 1
fi

$1 --build $2 --target ScriptsLibrary

read -n1 -s -r -p $'Press any key to continue...\n' key
