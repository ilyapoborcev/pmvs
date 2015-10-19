fork:
	mkdir -p bin
	gcc -DPROCESS server.c -o bin/server
	gcc client.c -o bin/client
thread:
	mkdir -p bin
	gcc -pthread server.c -o bin/server
	gcc client.c -o bin/client
clean:
	rm -rf bin
