compile:
	cc -o sta main.c

install: compile
	cp sta /usr/local/bin

uninstall:
	rm /usr/local/bin/sta
