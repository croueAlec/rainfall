#! /bin/bash

HOSTNAME="127.0.0.1"

if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ]; then
	echo "Missing argument: Usage: ./inject_file.sh [target user] [password file path] [file]"
	exit 1
fi

VM_USER="$1"
PASS_PATH="$2"

FILE="$3"

echo "$FILE $(basename $FILE)"

sshpass -f $PASS_PATH scp -P 4242 $FILE $VM_USER@$HOSTNAME:/tmp/$(basename $FILE) 2> /dev/null

if [ $? -ne 0 ]; then
	echo "Failed to copy $(basename $FILE) to /tmp/$(basename $FILE)"
	exit 1
fi

echo "Successfully copied $(basename $FILE) to /tmp/$(basename $FILE)"
