CC = gcc
CFLAGS = -fPIC
VPATH = lib

all: rootkitty kittylib.so
	@echo "ଘ(∩^o^)⊃━☆:･ﾟ✧*:･ﾟ✧✯♡❀"
	@echo "Successful compilation"

rootkitty: rootkitty.c
	$(CC) $(CFLAGS) -o rootkitty rootkitty.c

kittylib.so: kittylib.c
	$(CC) $(CFLAGS) -shared -o kittylib.so $(VPATH)/kittylib.c
	
clean:
	rm -f rootkitty kittylib.so
