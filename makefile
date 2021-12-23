
all: icmpPing sniffer

icmpPing: icmpPing.o
	gcc -Wall -g -o icmpPing icmpPing.o

sniffer: sniffer.o
	gcc -Wall -g -o sniffer sniffer.o

sniffer.o: sniffer.c
	gcc -Wall -g -c sniffer.c

icmpPing.o: icmpPing.c
	gcc -Wall -g -c icmpPing.c

.PHONY: clean all

clean:
	rm -f *.o *.a *.so icmpPing sniffer
