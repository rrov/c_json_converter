lib:
	cc -ansi -c cjc.c -o cjc.o
	ar rcs cjc.a cjc.o

build-test:
	cc test.c -ansi -L. -l:cjc.a -o test
