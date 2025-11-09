lib:
	cc -std=c17 -c cjc.c -o cjc.o
	ar rcs cjc.a cjc.o

build-test:
	cc test.c -std=c17 -L. -l:cjc.a -o test
