# Linux Administration tasks
This repo contains Linux administration tasks

## Task #1: Directories Creation
#### 1. bash script to make:
  - creating directories
  - copying and moving files
  - renaming directories

## Task #3: Modifying .bashrc File
#### 1. bash script to make:
  - checking if the file exists
  - append new environment variables in .bashrc file
  - opening a new terminal
#### 2. List the user commands and redirect the output to /tmp/commands.list
```bash
ls /bin > /tmp/commands.list
```
#### 3. command to search for all files on the system that its name is “.profile”.
```bash
sudo find / -name .profile
```
>> '/' to start the search from root directory <br>
>> '-name' to search by name

    
## Task #4: User and Group Creation
#### 1. bash script to make:
- creating new user and group
- append user to a new group
- printing relevant information for the user and group
  
#### 2. Create a supplementary (Secondary) group called pgroup with group ID of 30000
```bash
groupadd -g 30000 pgroup
```
>> '-g' used to make a group with specific ID

#### 3. Lock any user created account so he can't log in
```bash
passwd -l userName
```
#### 4. Delete user account & Delete group account
```bash
sudo userdel -r userName
sudo groupdel groupName
```
#### 5. State the difference between adduser and useradd:
- adduser: is native binary compiled with the system and more user friendly and interactive than its back-end useradd
- useradd: is a perl script which uses useradd binary in back-end.

## task #5: Permissions
### Section 1:
#### 1. Create a folder called myteam and change its permissions to read only for the owner
```bash
mkdir myteam | sudo chmod 400 myteam
```
#### 2. Log out and log in by another user
```bash
pkill -KILL -u userName
```

#### 3. Try to access (by cd command) the folder (myteam)
```bash
Not accessable for anyone (except root) 
```

#### 4. use command line
##### a- Change the permissions of oldpasswd file to give owner read and write permissions and for group write and execute and execute only for the others (using chmod in 2 different ways)
```bash
# using octal representation
sudo chmod 631 oldpasswd
# using symbolic representation
sudo chmod u=rw,g=wx,o=x oldpasswd
```
##### b. Change your default permissions to be as above
```bash
```
##### c. What is the maximum permission a file can have, by default when it is just created? And what is that for directory
- New files: 666 - 022 = 644
- New directories: 777 - 002 = 775

##### d. Change your default permissions to be no permission to everyone then create a directory and a file to verify.
> umask 777
 
##### e. State who can access this file if any with proof
> No one can access this file except the root user

#### 5. Create a file with permission 444. Try to edit in it and to remove it? Note what happened
> Cannot write onto the file or delete or execute as the permissions is read only for user, group, and others.

#### 7. What is the difference between the “x” permission for a file and for a directory?
> For directories means that we can access the directory (cd)
> for files means that we can execute this file (run a script)


### Section 2:
#### 1. Set the sticky bit on the newly created directory
```bash
sudo chmod +t test_dir
```
#### 2. Set the setgui bit on the created directory
```bash
to set user ID: sudo chmod u+s test_dir
to set group ID: sudo chmod g+s test_dir
```
#### 3. Create multiple user accounts.
```bash
sudo adduser user1
sudo adduser user2
```


## Task #6: For Loop
#### 1. bash script to make:
  - creating multiple files
  - append date to the created files
  - print the content of the files using for loops
