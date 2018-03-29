username = $(shell whoami)
SRCDIR = src
files = $(SRCDIR)/main.c
opflag = -o editor

all: run

run: compile
	./editor

debugrun: compile
	valgrind --track-origins=yes ./editor

compile:
	gcc -Wall -g $(files) $(opflag)