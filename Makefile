lib:
	clang cjc.c -o cjc

test:
	clang test.c -o test

clean:
	rm -f cjc
