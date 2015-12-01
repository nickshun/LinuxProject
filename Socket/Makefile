####
#
# Makefile
#
# Main makefile to build: - ASCII character device driver
#                         - main() to test the driver
#
# Serguei Mokhov
#
##

CC=gcc
DEBUG=-g -D_DEBUG
DEFINE=-DMODULE -D__KERNEL__ -DLINUX
WARNINGS=-Wall -Wmissing-prototypes -Wmissing-declarations
#ISO=-ansi -pedantic
CC_OPTIONS=-O1 $(WARNINGS) $(ISO) $(DEBUG) $(DEFINE)

# Where to look for header files
INC=-I. -I/usr/include -I/usr/src/kernels/`uname -r`/include

all: $(EXE)
	gcc mapclient.c -o client
	gcc mapserver.c -o server

clean:
	rm -f $(EXE) $(OBJ)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

compile: $(EXE) $(OBJ)

$(EXE): server.o client.o
	$(CC) server.o -o server
	$(CC) client.o -o client
	
test:
	./client localhost.localdomain

server.o: mapserver.c header.h
	$(CC) $(CC_OPTINS) $(INC) -c mapserver.c

client.o: mapclient.c header.h
	$(CC) $(CC_OPTINS) $(INC) -c mapclient.c

$(DRIVER): types.h mapdriver.h mapdriver.c
	$(CC) $(CC_OPTIONS) $(INC) -c mapdriver.c

clean-all:
	make clean
	lsmod
	rm client
	rm server

# EOF
