#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ]; then
  echo "Error : three arguments are needed."
  echo "Usage : $0 <cmake_path> <build_path> <script_library_target_name>"
  exit 1
fi

$1 --build $2 --target $3

read -n1 -s -r -p $'Press any key to continue...\n' key
