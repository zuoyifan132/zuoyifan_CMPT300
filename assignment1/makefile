x:	test.o list.o
	gcc -Wall -Werror -o x instructorTest.o list.o

test.o:	instructorTest.c list.c
	gcc -c instructorTest.c

list.o:	instructorTest.c list.c
	gcc -c list.c

clean:
	rm -f *.o x