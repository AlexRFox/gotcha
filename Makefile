CFLAGS=-g -Wall
LIBS=-lX11

gotcha: gotcha.o
	$(CC) $(CFLAGS) -o gotcha gotcha.o $(LIBS)

clean:
	rm -f *.o
	rm -f *~
	rm -f gotcha
