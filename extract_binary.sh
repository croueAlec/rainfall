#! /bin/bash

if [ -z "$1" ]; then
	echo "Error : No user binary specified";
	exit 1
else
	VM_USER="$1"
	if [[ "$VM_USER" =~ ^(level[0-9]|bonus[0-3])$ ]]; then
		echo "Extracting $VM_USER"

		mkdir -p binaries/
		scp -P 4242 $VM_USER@localhost:/home/user/$VM_USER/$VM_USER ./binaries/.

		exit 0
	else
		echo "Error : Invalid user binary"
		exit 1
	fi
fi

