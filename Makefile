CC=gcc
CFLAGS=-Wall

all: exam eval

exam: exam.o
eval: eval.o

clean:
	rm -f exam eval
	rm -f *.o

clean-all:
	rm -f exam eval
	rm -f *.o
	rm -f *~
