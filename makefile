gcc -o C ./client/client.c -lpthread
gcc -o S ./server/server.c ./include/buff.c -lpthread
