#!/bin/bash

# Display the prompt
while true; do
    read -p "$ " input

    # Check for end of file (Ctrl+D)
    if [ -z "$input" ]; then
        echo "Goodbye!"
        exit 0
    fi

    # Execute the command
    eval "$input"
done