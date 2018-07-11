CC = gcc

CFLAGS = -Wtype-limits
CFLAGS += -Wcast-align
CFLAGS += -Wstrict-overflow
CFLAGS += -Wformat-security
CFLAGS += -Wformat
CFLAGS += -Wstrict-aliasing=3
CFLAGS += -Wall
CFLAGS += -Wcast-align

SRC = ./src/ex2_main.c

all : $(SRC)  
	$(CC) $(CFLAGS) -o ex2.exe $(SRC)
	@echo Press any key to run...
	@read dummy
	@./ex2.exe
	
clean :
	rm -f *.exe