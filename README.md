# 4610Shell

COP4610
Project 3: FAT32

Team Members:
	Houston Luzader
	Brandon Matulonis

Division of Labor:
	Houston Luzader
		-Part 1 : exit
		-Part 3 : size
		-Part 5 : cd
		-Part 6 : creat
		-Part 7 : mkdir
		-Part 9 : open
		-Part 10: close
	Brandon Matulonis
		-Part 2 : info
		-Part 4 : ls
		-Part 8 : mv
		-Part 13: rm
		-Part 14: cp
		-Part 15: rmdir

Tar contents:
	-log.txt : Git commit log
	-src/ 	 : source code

How to compile executables:
	-place image in parent directory of src
	-go inside of src directory
	>make
	>make run

Known bugs:
	-creat and mkdir are hardcoded to only be inside of the first free
	 cluster, so directories longer than 16 entries that are made via
	 mkdir may be overwritten when the next time mkdir or creat are called.
	-rm and rmdir might not deallocate all memory, and might only remove
	 the entry from the directory cluster instead.

Unfinished sections:
	-read
	-write

Extra credit:
	-rmdir
