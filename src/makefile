C=gcc -g
L=gcc -g -c

../proj3.x : main.o BPB.o ls.o size.o cd.o creat.o mv.o cp.o rm.o
	$C main.o BPB.o creat.o cd.o ls.o size.o mv.o cp.o rm.o -o ../proj3.x
	chmod -R 700 ..

main.o : main.c main.h
	$L main.c

creat.o : creat.c creat.h
	$L creat.c

cd.o : cd.c cd.h
	$L cd.c

mv.o : mv.c mv.h
	$L mv.c

cp.o : cp.c cp.h
	$L cp.c

rm.o : rm.c rm.h
	$L rm.c

BPB.o : BPB.c BPB.h
	$L BPB.c

size.o : size.c size.h
	$L size.c

ls.o : ls.c ls.h
	$L ls.c

reset :
	tar -xf ../fat32.img.tar.gz
	mv fat32.img ..

run :
	../proj3.x ../fat32.img

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
