#! /bin/bash

PASS_PATH=""
HOSTNAME="127.0.0.1"

function print_extraction() {
	bold_value="\033[1m$1\033[0m"
	echo -e "Extracting ./$bold_value from user $bold_value to ./binaries/$bold_value"
}

function set_pass_path() {
	previous_user="$1"

	if [ ! -s "./$previous_user/flag" ]; then
		echo "Error : Flag has not been found yet, go back to work."
		exit 1
	fi
	print_extraction $VM_USER; 
	PASS_PATH="./$previous_user/flag"
}

if [ -z "$1" ]; then
	echo "Error : No user specified";
	exit 1
else
	VM_USER="$1"
	if [[ "$VM_USER" =~ ^(level[0-9]|bonus[0-3])$ ]]; then

		num=${VM_USER##*[!0-9]}
		prefix=${VM_USER%"$num"}

		previous_user="${prefix}$((num - 1))"

		case "$VM_USER" in
		"level0") print_extraction $VM_USER; PASS_PATH="./tools/.level0_flag"
		;;
		"bonus0") set_pass_path "level9"
		;;
		*) set_pass_path "$previous_user"
		esac

		sshpass -f $PASS_PATH scp -P 4242 $VM_USER@$HOSTNAME:/home/user/$VM_USER/$VM_USER ./binaries/. 2> /dev/null
		echo "Extract done"
	else
		echo "Error : Invalid user name"
		exit 1
	fi
fi
