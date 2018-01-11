CC = gcc
CFLAGS = -Wall -Werror
OBJECTS = Game.o runGame.o ai.o

goFish : Game.o runGame.o ai.o
	 $(CC) -o goFish $(CFLAGS) $(OBJECTS)

Game.o : Game.c
	 cc -c Game.c

runGame.o : runGame.c
	    cc -c runGame.c

ai.o : ai.c
	cc -c ai.c

clean : 
	rm -f *.o goFish
