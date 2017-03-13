#the compiler: gcc for C program, define as g++ for C++
CC = gcc

#compiler flags:
# -lcurses use curses to create win and motion
# -lpthread use thread to do concurrent frogram
CFLAGS = -lcurses -lpthread

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
default: bounce2dc

# To create the executable file count we need the object files
# bounce2ds.o, bounce2dc.o:
#
bounce2dc: bounce2dc.o bclient.o paddle.o
	$(CC) -o bounce2dc bounce2dc.o bclient.o paddle.o $(CFLAGS)

# To create the object file bounce2ds.o, we need the source
# files bserver.c, bounce.h, and paddle.c:
#
bounce2dc.o: bounce2dc.c bclient.c paddle.c bounce.h
	$(CC) -c bounce2dc.c bclient.c paddle.c $(CFLAGS)

# To create the object file bounce2dc.o, we need the source files
# bclient.c, paddle.c and bounce.h:
#
bserver.o:  bclient.c bounce.h 
	$(CC) -c bclient.c $(CFLAGS)

paddle.o: paddle.c bounce.h
	$(CC) -c paddle.c $(CFLAGS)
# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean:
	rm -rf *.o
