username = $(shell whoami)
SRCDIR = src
files = $(SRCDIR)/main.c
opflag = -o editor

all: run

run: compile
	./editor $(ARGS)

debugrun: compile
	valgrind --track-origins=yes ./editor $(ARGS)

compile:
	gcc -Wall -g $(files) $(opflag)