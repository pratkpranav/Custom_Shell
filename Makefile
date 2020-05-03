master.exe: shell.exe master.c
	gcc master.c -o master.exe


shell.exe: shell.c
	gcc shell.c -o shell.exe -L/usr/local/lib -I/usr/local/include -lreadline


run: master.exe
	./master.exe


clean: shell.exe master.exe
	rm shell.exe
	rm master.exe








	