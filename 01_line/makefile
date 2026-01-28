OBJECTS= main.o draw.o display.o
STAR_OBJS = star.o turtle.o draw.o display.o
CFLAGS= -Wall
CC= gcc

run: main
	./main

main: $(OBJECTS)
	$(CC) -o main $(OBJECTS)

star: $(STAR_OBJS)
	$(CC) -o star $(STAR_OBJS) -lm

main.o: main.c display.h draw.h ml6.h
	$(CC) -c main.c

draw.o: draw.c draw.h display.h ml6.h
	$(CC) $(CFLAGS) -c draw.c

display.o: display.c display.h ml6.h
	$(CC) $(CFLAGS) -c display.c

turtle.o: turtle.c turtle.h ml6.h
	$(CC) $(CFLAGS) -c turtle.c

star.o: star.c turtle.h ml6.h
	$(CC) $(CFLAGS) -c star.c

clean:
	rm *.o *~
