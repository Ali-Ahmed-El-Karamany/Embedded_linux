# Linux Administration tasks
This repo contains Linux administration tasks

## task_1: directories creation
#### 1. bash script to make:
  - creating directories
  - copying and moving files
  - renaming directories

## task_3: modifying .bashrc file
#### 1. bash script to make:
  - checking if the file exists
  - append new environment variables in .bashrc file
  - opening a new terminal
#### 2. List the user commands and redirect the output to /tmp/commands.list

```
ls /bin > /tmp/commands.list
```
#### 3. command to search for all files on the system that its name is “.profile”.
```
sudo find / -name .profile
```
>> '/' to start the search from root directory <br>
>> '-name' to search by name

    
## task_4: user and group creation
#### 1. bash script to make:
- creating new user and group
- append user to a new group
- printing relevant information for the user and group
  
#### 2. Create a supplementary (Secondary) group called pgroup with group ID of 30000
```
groupadd -g 30000 pgroup
```
>> '-g' used to make a group with specific ID

#### 3. Lock any user created account so he can't log in
```
passwd -l userName
```
#### 4. Delete user account & Delete group account
```
sudo userdel -r userName
sudo groupdel groupName
```
#### 5. State the difference between adduser and useradd:
- adduser: provides guides through user creation process
- useradd: provides complete control through user creation process



## task_6: for loop
#### 1. bash script to make:
  - creating multiple files
  - append date to the created files
  - print the content of the files using for loops


