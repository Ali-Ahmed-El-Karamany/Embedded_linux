# Bash scripts 

## 1. Automated backups

Script takes a list of directories as arguments and creates a compressed backup (tarball) of each specified directory

- The script accept one or more directory paths as command-line arguments.
```bash
./backup_script.sh /path/to/directory1 /path/to/directory2 ...
```
- creates a compressed backup file in the format backup_<directory>_<date>.tar.gz.
- all backup files placed in a backup directory named backups within the script's working directory.
- The script creates the backups directory if it does not exist.



