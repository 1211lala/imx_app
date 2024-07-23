obj: 
	@echo $(shell pwd)
main: hello.o  main.o 
	gcc main.o hello.o  -o main

main.o:	main.c hello.h
	gcc -c main.c 
hello.o: hello.c hello.h
	gcc -c hello.c 

clean:
	-rm *.o