CC:=gcc
CFLAGS:=`curl-config  --cflags`   -g
LDFLAGS:=`curl-config --libs`  

bins:=gethttp getCDN

all:$(bins)
gethttp:gethttp.o url.o
	$(CC) $(LDFLAGS) -o $@ $^
getCDN:getCDN.o url.o net.o
	$(CC) $(LDFLAGS) -o $@ $^
gethttp.o:gethttp.c
	$(CC) -c $(CFLAGS)  $^
getCDN.o:getCDN.c
	$(CC) -c $(CFLAGS) $^
url.o:url.c
	$(CC) -c $(CFLAGS) -std=c99 -Wall   $^
net.o:net.c
	$(CC) -c $(CFLAGS) $^

clean:
	rm  -rf *.o&&rm $(bins)

	
