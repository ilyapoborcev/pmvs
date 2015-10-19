PROCESS=0

all:
	mkdir -p bin
	gcc -DPROCESS=$(PROCESS) -pthread server.c -o bin/server
	gcc client.c -o bin/client

clean:
	rm -rf bin
