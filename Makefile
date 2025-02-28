pmalloc:
	gcc -g -o pmalloc pmalloc.c

all:
	(cd tests && make)

clean:
	(cd tests && make clean)
	rm -f valgrind.out stdout.txt stderr.txt *.plist


