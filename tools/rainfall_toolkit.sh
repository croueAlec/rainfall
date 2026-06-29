#! /bin/bash

PASS_PATH=""
HOSTNAME="127.0.0.1"
PROJECT_NAME="Rainfall"

COMMAND="$1"
VM_USER="$2"

FILE_INJECT="$3"

project_name_bold="\033[1m$PROJECT_NAME\033[0m"
vm_user_bold="\033[1m$VM_USER\033[0m"
filename_bold="\033[1m$FILE_INJECT\033[0m"

function print_connection() {
	echo -e "Logging in to $project_name_bold as user $vm_user_bold"
}


function print_injection() {
	echo -e "Injecting $filename_bold to user $vm_user_bold in /tmp/$filename_bold"
}


find_latest_level() {
	for user in level0 level1 level2 level3 level4 level5 \
				level6 level7 level8 level9 bonus0 bonus1 \
				bonus2
	do
		if [ ! -s "./$user/flag" ]; then
			VM_USER="$user"
			vm_user_bold="\033[1m$VM_USER\033[0m"
			return
		fi
	done
}

function open_user() {
	print_connection
	sshpass -f $PASS_PATH ssh -p 4242 $VM_USER@$HOSTNAME
}


function inject_file() {
	if [ -z "$FILE_INJECT" ]; then
		echo "Error : Missing inject file argument : Usage : ./rainfall_toolkit.sh inject <target user> <source file path>"
		exit 1
	fi

	sshpass -f $PASS_PATH scp -P 4242 $FILE_INJECT $VM_USER@$HOSTNAME:/tmp/$(basename $FILE_INJECT) 2> /dev/null

	if [ $? -ne 0 ]; then
		echo "Failed to copy $(basename $FILE_INJECT) to /tmp/$(basename $FILE_INJECT)"
		exit 1
	fi

	print_injection "$(basename $FILE_INJECT)"
}


function extract_file() {
	sshpass -f $PASS_PATH scp -P 4242 $VM_USER@$HOSTNAME:/home/user/$VM_USER/$VM_USER ./binaries/. 2> /dev/null

	if [ $? -ne 0 ]; then
		echo -e "Failed to extract $vm_user_bold to ./binaries/$vm_user_bold"
		exit 1
	fi

	echo -e "Successfully extracted $vm_user_bold to ./binaries/$vm_user_bold"
}


function set_pass_path() {
	if [[ "$VM_USER" =~ ^(level0)$ ]]; then
		PASS_PATH="./tools/.level0_flag";
		return ;
	fi

	previous_user="$1"

	if [ ! -s "./$previous_user/flag" ]; then
		echo "Error : Flag has not been found yet, go back to work."
		exit 1
	fi
	PASS_PATH="./$previous_user/flag"
}


if [ -z "$COMMAND" ]; then
	echo "Error : Missing argument : Usage : ./rainfall_toolkit.sh <open|inject|extract> <target user> [file path]";
	exit 1
else
	if [[ "$VM_USER" =~ ^(latest)$ ]]; then
		find_latest_level
	fi

	if [[ "$VM_USER" =~ ^(level[0-9]|bonus[0-3])$ ]]; then

		num=${VM_USER##*[!0-9]}
		prefix=${VM_USER%"$num"}

		previous_user="${prefix}$((num - 1))"

		case "$VM_USER" in
		"bonus0") set_pass_path "level9"
		;;
		*) set_pass_path "$previous_user"
		esac

		case "$COMMAND" in
		"open") open_user;
		;;
		"inject") inject_file;
		;;
		"extract") extract_file;
		;;
		*) echo "Error : Invalid command : Usage : ./rainfall_toolkit.sh <open|inject|extract> <target user> [source file path]"; exit 1;
		esac

	else
		echo "Error : Invalid user name"
		exit 1
	fi
fi
