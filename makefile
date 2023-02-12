all: time_client time_server

clean:
	rm -f time_server.o time_client.o time_client time_server

time_server: time_server.o
	gcc time_server.o -o time_server

time_server.o: time_server.c
	gcc -c time_server.c

time_client: time_client.o
	gcc time_client.o -o time_client

time_client.o: time_client.c
	gcc -c time_client.c
