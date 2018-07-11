CC = gcc

LDLIBS = -lpthread

CFLAGS = -Wtype-limits
CFLAGS += -Wcast-align
CFLAGS += -Wstrict-overflow
CFLAGS += -Wformat-security
CFLAGS += -Wformat
CFLAGS += -Wstrict-aliasing=3
CFLAGS += -Wall
CFLAGS += -Wcast-align

SRC = ./src/ex1_main.c
SRC += ./common/src/utils.c


all : $(SRC)  
	$(CC) $(CFLAGS) $(LDLIBS) -o ex1.exe $(SRC)
	@echo Press any key to run...
	@read dummy
	@./ex1.exe
clean :
	rm -f *.exe
