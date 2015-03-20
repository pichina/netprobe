CC:=gcc
CFLAGS:=`curl-config  --cflags`   -g
LDFLAGS:=`curl-config --libs`   -lsqlite3

bins:=gethttp getCDN

all:$(bins)
gethttp:gethttp.o 
	$(CC) $(LDFLAGS) -o $@ $^
getCDN:getCDN.o net.o url_parser.o io_console.o io_sqlite.o
	$(CC) $(LDFLAGS) -o $@ $^
gethttp.o:gethttp.c
	$(CC) -c $(CFLAGS)  $^
getCDN.o:getCDN.c
	$(CC) -c $(CFLAGS) $^
net.o:net.c
	$(CC) -c $(CFLAGS) $^
url_parser.o:url_parser.c
	$(CC) -c $(CFLAGS) $^
io_console.o:io_console.c
	$(CC) -c $(CFLAGS) $^
io_sqlite.o:io_sqlite.c
	$(CC) -c $(CFLAGS) $^


clean:
	rm  -rf *.o&&rm $(bins)

	
