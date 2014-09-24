CS3210HW1
=========

Linux Kernel Module to translate morse code


Caven Diary:
1. make sure the encodeMorse script is executable and add it to /usr/bin
Feature to add: 
	when the module is not installed(proc file doesn't exsit), show help info
	>>encodeMorse --help # or -h to show the hint for use

2. encode part is partially done; haven't filled all the charToBin;

3. how to test?
>> sudo make
>> sudo make test
>> sudo make print | grep DBG