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

SRC 	= ./src/ex3_main.c
SRC_CH	= ./src/ex3_child.c 

all :
	@mkdir logs
	$(CC) $(CFLAGS) $(LDLIBS) -o ex3.exe $(SRC)
	$(CC) $(CFLAGS) $(LDLIBS) -o ex3_child.exe $(SRC_CH)
	
clean :
	@rm -f *.exe 
	@rm -rf ./logs
