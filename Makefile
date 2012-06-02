MARCO=Doing all
#FLAGS=-ansi -pedantic -W -Wall -Wundef -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -O2
FLAGS=-pedantic -W -Wall -Wundef -Wmissing-prototypes -Wmissing-declarations -O2
SOURCES=mongrel_process.c
all:
	echo $(MACRO)
	gcc $(FLAGS) $(SOURCES) -o mongrel_process


clean:
	rm -rf *.o
	
