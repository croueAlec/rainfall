#!/bin/bash

function math() {

	# Helper functions
	dec_to_bin() {
		n=$1
		bin=""

		while [ "$n" -gt 0 ]; do
			bin="$((n % 2))$bin"
			n=$((n / 2))
		done

		[ -z "$bin" ] && bin=0

		while [ $(( ${#bin} % 4 )) -ne 0 ]; do
			bin="0$bin"
		done

		printf "0b%s" "$bin"
	}

	to_decimal() {
		case "$1" in
			0x*) printf "%d" "$((16#${1#0x}))"
			;;
			0b*) printf "%d" "$((2#${1#0b}))"
			;;
			0o*) printf "%d" "$((8#${1#0o}))"
			;;
			0d*) printf "%d" "$((10#${1#0d}))"
			;;
			*)   printf "%d" "$((10#$1))"
		esac
	}

	dec_to_hex() {
		printf "0x%x" "$1"
	}

	dec_to_octal() {
		printf "0o%o" "$1"
	}

	dec_to_decimal() {
		printf "%d" "$1"
	}

	print_handler() {
		case "$2" in
			0x) dec_to_hex "$1" ;;
			0b) dec_to_bin "$1" ;;
			0o) dec_to_octal "$1" ;;
			0d) printf "0d%d" "$1" ;;
			*) dec_to_decimal "$1" ;;
		esac
	}

	check_operation_arguments() {
		if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ]; then
			printf "bad2"
		else
			a="$(to_decimal "$2")"
			b="$(to_decimal "$3")"
			case "$1" in
			"add") result="$(( $a + $b ))"
			;;
			"sub") result="$(( $a - $b ))"
			;;
			*) 
			esac

			print_handler "$result" "$4"
		fi
	}

	# Beginning of the math function
	if [ -z "$1" ]; then
		echo "Error : Missing argument : Usage : math <value 0|add|sub> <value 1> <value 2> <0d|0x|0o|0b>"
		return 1
	else
		case "$1" in
		"add"|"sub") check_operation_arguments "$1" "$2" "$3" "$4"
		;;
		*) print_handler "$(to_decimal "$1")" "$2"
		esac

		printf "\n"
	fi
}
