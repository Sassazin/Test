all : 
	gcc -Wtype-limits -Wcast-align -Wstrict-overflow -Wformat-security -Wformat -Wstrict-aliasing=3 -Wall -Wcast-align -lpthread -o ex3.exe ./src/ex3_main.c
	gcc -g -Wtype-limits -Wcast-align -Wstrict-overflow -Wformat-security -Wformat -Wstrict-aliasing=3 -Wall -Wcast-align -lpthread -o ex3_child.exe ./src/ex3_child.c
	@echo "make: built targets"

clean :
	@echo "make: removing targets and objects of `pwd`"
