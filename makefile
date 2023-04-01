major2: main.o
	gcc -o major2 main.o

main.o: main.c func.h
	gcc -c main.c  

clean:
	rm major2
