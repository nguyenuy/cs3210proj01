CS3210HW1
=========

Linux Kernel Module to translate morse code

#####<u>To Do</u>
1. Account for spaces in Morse code ==> dit-dit-dit-dit-dit-dit-dit (7 dits total)
2. Finalize memory management scheme
3. Create shell script that wraps around kernel modules
4. When encoding, make sure to ignore Upper case characters. A tolower() should take care of the trick.


#####<u>Caven Diary:</u>

1. add scripts to /usr/bin and chmod +x; 
2. Feature to add: 
	when the module is not installed(proc file doesn't exsit), show help info
	>encodeMorse --help # or -h to show the hint for use


Compiled on Ubuntu against Linux kernel v 3.11

