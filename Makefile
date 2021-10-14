CC=gcc
CFLAGS=-g

SRCDIR=src
INCLDIR=include
LIBDIR=lib
TESTDIR=test

master: $(SRCDIR)/master.c $(LIBDIR)/utils.o $(LIBDIR)/myutils.o childProgram
	$(CC) $(CFLAGS) -I$(INCLDIR) $(SRCDIR)/master.c $(LIBDIR)/utils.o $(LIBDIR)/myutils.o -o master

childProgram: $(SRCDIR)/childprogram.c $(LIBDIR)/utils.o
	$(CC) $(CFLAGS) -I$(INCLDIR) $(SRCDIR)/childprogram.c $(LIBDIR)/utils.o $(LIBDIR)/myutils.o -o childProgram

$(LIBDIR)/myutils.o: $(SRCDIR)/myutils.c
	$(CC) $(CFLAGS) -I$(INCLDIR) -c $(SRCDIR)/myutils.c -o $(LIBDIR)/myutils.o

.PHONY: clean run1 run2 run3 run4 run5 run6 run7 run8 run9

run1:
	./master input1.file

run2:
	./master input2.file

run3:
	./master input3.file

run4:
	./master input4.file

run5:
	./master input5.file

run6:
	./master input6.file

run7:
	./master input7.file

run8:
	./master input8.file

run9:
	./master input9.file

fresh: clean cleanTests

clean:
	rm lib/myutils.o master childProgram
	rm -rf output
	rm -rf *.dSYM

tests: testMyutils

testMyutils: test1 test2 test3

test1: $(TESTDIR)/testsExe/myutils_tests
	$(TESTDIR)/testsExe/myutils_tests < $(TESTDIR)/input/i1.file > $(TESTDIR)/actual/a1.file
	diff $(TESTDIR)/expected/e1.file $(TESTDIR)/actual/a1.file

test2: $(TESTDIR)/testsExe/myutils_tests
	$(TESTDIR)/testsExe/myutils_tests < $(TESTDIR)/input/i2.file > $(TESTDIR)/actual/a2.file
	diff $(TESTDIR)/expected/e2.file $(TESTDIR)/actual/a2.file

test3: $(TESTDIR)/testsExe/myutils_tests
	$(TESTDIR)/testsExe/myutils_tests < $(TESTDIR)/input/i3.file > $(TESTDIR)/actual/a3.file
	diff $(TESTDIR)/expected/e3.file $(TESTDIR)/actual/a3.file

$(TESTDIR)/testsExe/myutils_tests: $(TESTDIR)/testsSrc/myutils_tests.c $(LIBDIR)/utils.o $(LIBDIR)/myutils.o
	$(CC) $(CFLAGS) -I$(INCLDIR) $(TESTDIR)/testsSrc/myutils_tests.c $(LIBDIR)/utils.o $(LIBDIR)/myutils.o -o $(TESTDIR)/testsExe/myutils_tests

cleanTests:
	rm $(TESTDIR)/actual/*.file
	rm $(TESTDIR)/testsExe/myutils_tests