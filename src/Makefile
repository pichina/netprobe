CC:=gcc
CFLAGS:= -g
LDFLAGS:=-lcurl   -lsqlite3

bins:=gethttp getCDN mping url_parser_tool

all:$(bins)
mping:ping.o io_console.o io_sqlite.o
	$(CC) $(LDFLAGS) -o $@ $^
gethttp:gethttp.o net.o url_parser.o io_console.o io_sqlite.o
	$(CC) $(LDFLAGS) -o $@ $^
getCDN:getCDN.o net.o url_parser.o io_console.o io_sqlite.o
	$(CC) $(LDFLAGS) -o $@ $^
url_parser_tool:url_parser_tool.o url_parser.o
	$(CC) $(LDFLAGS) -o $@ $^
url_parser_tool.o:url_parser_tool.c
	$(CC) -c $(CFLAGS)  $^
gethttp.o:gethttp.c
	$(CC) -c $(CFLAGS)  $^
getCDN.o:getCDN.c
	$(CC) -c $(CFLAGS) $^
net.o:net.c
	$(CC) -c $(CFLAGS) $^
url_parser.o:url_parser.c
	$(CC) -c $(CFLAGS) $^
ping.o:ping.c
	$(CC) -c $(CFLAGS) $^
io_console.o:io_console.c
	$(CC) -c $(CFLAGS) $^
io_sqlite.o:io_sqlite.c
	$(CC) -c $(CFLAGS) $^


clean:
	rm  -rf *.o&&rm $(bins)

	
