LIBHTTP_PATH=./civetweb

CLIBS=-lpthread -ldl
CFLAGS=-I$(LIBHTTP_PATH)include

CIVETWEB_LIB = libcivetweb.a

CC=g++


all: platform publisher subscriber

platform: $(CIVETWEB_LIB)
	$(CC) platform.cpp utils.cpp ./cJSON/cJSON.c -o platform $(CIVETWEB_LIB) $(CLIBS) $(CFLAGS)

publisher: $(CIVETWEB_LIB)
	$(CC) publisher.cpp utils.cpp ./cJSON/cJSON.c -o publisher $(CIVETWEB_LIB) $(CLIBS) $(CFLAGS)

subscriber: $(CIVETWEB_LIB)
	$(CC) subscriber.cpp utils.cpp ./cJSON/cJSON.c -o subscriber $(CIVETWEB_LIB) $(CLIBS) $(CFLAGS)

$(CIVETWEB_LIB):
	$(MAKE) -C $(LIBHTTP_PATH) clean lib
	cp $(LIBHTTP_PATH)/$(CIVETWEB_LIB) .

debug: $(CIVETWEB_LIB) functions.c cflask.c
	$(CC) -g -o cflask cflask.c functions.c  $(CIVETWEB_LIB) $(CLIBS) $(CFLAGS)

clean:
	rm -f platform publisher subscriber

.PHONY: all clean
