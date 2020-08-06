all: myls.o print.o stringProcess.o
	gcc -Wall -Werror myls.o print.o stringProcess.o -o myls

myls.o:	myls.c
	gcc -c myls.c

print.o: print.c 
	gcc -c print.c

stringProcess.o: stringProcess.c
	gcc -c stringProcess.c

clean:
	rm -f *.o myls