CC	= g++
CFLAGS	= 

TARGET1	= frontEnd
OBJS1	= main.o
OBJS2	= scanner.o
OBJS3	= parser.o
OBJS4 	= testTree.o
CXXFLAGS = -std=c++11

.SUFFIXES: .c .o

all:	$(TARGET1)

frontEnd:	$(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4)
		$(CC) $(CFLAGS) $(CXXFLAGS) $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) -o $@


.c.o:
		$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	/bin/rm -f *.o *.dat  *~ $(TARGET1)
