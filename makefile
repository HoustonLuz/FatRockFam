C=gcc -g
L=gcc -g -c

shell : parser.c
	$C parser.c -o shell
	chmod 700 *

clean:
	rm -f *.tar
	rm -f *.txt

tar:
	git log > log.txt
	tar -cf project3_luzader_matulonis.tar README.md log.txt makefile
