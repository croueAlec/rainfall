#! /bin/bash

function print_extraction() {
	bold_value="\033[1m$1\033[0m"
	echo -e "Extracting ./$bold_value from user $bold_value"
	echo -n "Password is : "
}

function print_bold() {
	echo -e "\033[1m$1\033[0m"
}

function print_flag() {
	previous_user="$1"

	if [ ! -s "./$previous_user/flag" ]; then
		echo "Error : Flag has not been found yet, go back to work."
		exit 1
	fi
	print_extraction $VM_USER; 
	print_bold $(cat ./$previous_user/flag)
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
		"level0") print_extraction $VM_USER; print_bold "level0"
		;;
		"bonus0") print_flag "level9"
		;;
		*) print_flag "$previous_user"
		esac

		scp -P 4242 $VM_USER@localhost:/home/user/$VM_USER/$VM_USER ./binaries/. 2> /dev/null
	else
		echo "Error : Invalid user name"
		exit 1
	fi
fi
