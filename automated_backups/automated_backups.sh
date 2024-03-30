#!/bin/bash

# checks if there is at least one argument
if [ $# -lt 1 ]; then
    echo "Usage: $0 [directory1] [directory2] ..."
    exit 1
fi

# create back up directory if it does not exist
backup_dir="backups"
if [ ! -d "$backup_dir" ]; then
	echo "backups directory does not exist... making backups directory"
	mkdir "$backup_dir"
fi


# loop through input  directoris 
for dir in "$@" ; do
	# check if the directory exist
	if [ ! -d "$dir" ]; then
	       	echo "Directory '$dir' doesnt exist"
		continue
	fi
	
	# Get current date
	current_date="$(date '+%d-%m-%y')"
	
	# Create backup filename
	filename="backup_$(basename "$dir")_${current_date}.tar.gz"
	
	# Create compressed backup
	tar -czf "$backup_dir/$filename" "$dir" && echo "Backup of '$dir' created successfully: $filename" || echo "Failed to create backup of '$dir'"
	
done




