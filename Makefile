all: termcrack

termcrack: termcrack.c
	gcc -lncurses -o termcrack termcrack.c

debug: clean
	gcc -g -DDEBUG=1 -lncurses -o termcrack termcrack.c

clean:
	@rm -f termcrack
