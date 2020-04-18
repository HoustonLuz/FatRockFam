C=gcc -g
L=gcc -g -c

../proj3.x : main.o BPB.o
	$C main.o BPB.o -o ../proj3.x
	chmod -R 700 ..

main.o : main.c main.h
	$L main.c

BPB.o : BPB.c BPB.h
	$L BPB.c

run :
	../proj3.x ../fat32.img > proj3.o
	chmod -R 700 ..

clean :
	rm -f ../proj3.x
	rm -f ../*.tar
	rm -f ../*.txt
	rm -f *.o

tar :
	make clean
	git log > ../log.txt
	tar -cf project3_luzader_matulonis.tar ../README.md ../log.txt ../src
	mv project3_luzader_matulonis.tar ..
	chmod -R 700 ..
