lib:
	cc -static -ansi -c cjc.c -o cjc.o
	ar rcs cjc.a cjc.o

lib-debug:
	cc -g -O0 -static -ansi -c cjc.c -o cjc-debug.o
	ar rcs cjc-debug.a cjc-debug.o

build-test:
	cc test.c -ansi -L. ./cjc.a -o test

build-test-debug:
	cc -g -O0 test.c -ansi -L. ./cjc-debug.a -o test-debug

run-test:
	make lib
	make build-test
	./test

