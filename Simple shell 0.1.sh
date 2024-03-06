#!/bin/bash

while true; do
	# Display prompt
	echo -n "$ "

	# Read command from user
	read command

	# Check for end of file (Ctrl+D)
	if [ -z "$command" ]; then
		echo
		break
	fi

	# Execute the command
	$command

	# Check exit status
	if [ $? -ne 0 ]; then
		echo "Error: Command '$command' returned non-zero exit status $?"
	fi
done
