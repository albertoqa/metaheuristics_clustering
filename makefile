CFLAGS = -g -c

C = main.c input.c clustering.c

O = main.o input.o clustering.o

.c.o:
	gcc $(CFLAGS) $(C)

main: main.o $(O)
		gcc -o main $(O) -lm
