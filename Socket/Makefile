all: client server

client:client.c
	gcc -o $@  $< -std=c99
server:server.c
	gcc -o $@  $< -std=c99 -lpthread

clean :
	rm -rf client server
