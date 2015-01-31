CC:=gcc
CFLAGS:=`curl-config  --cflags`   -g
LDFLAGS:=`curl-config --libs`  

bins:=gethttp

all:$(bins)
gethttp:gethttp.o
	$(CC) $(LDFLAGS) -o $@ $< 
gethttp.o:gethttp.c
	$(CC) -c $(CFLAGS)  $<
clean:
	rm  -rf *.o&&rm $(bins)

	
