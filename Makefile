
TARGET = libiopctaskhttp.so

CFLAGS += -I$(SDKSTAGE)/usr/include/
CFLAGS += -I$(SDKSTAGE)/usr/include/iopccommon

LDFLAGS += -L$(SDKSTAGE)/usr/lib/ 
LDFLAGS += -liopccommon -lmicrohttpd
LDFLAGS += -lc -lpthread 

SRC = 
SRC += main.c

include Makefile.include.lib
