# myps: myps.o options.o parse.o process.o
# 	gcc -o myps myps.o options.o parse.o process.o

# myps.o: myps.c options.h parse.h process.h structs.h
# 	gcc -c myps.c -Wall -Werror

# options.o: options.c 
# 	gcc -c options.c -Wall -Werror

# parse.o: parse.c 
# 	gcc -c parse.c -Wall -Werror

# process.o: process.c
# 	gcc -c process.c -Wall -Werror

# clean: 
# 	rm -rf *.o mymyps



# shell: shell.o pwd.o helpers.o cd.o help.o
# 	gcc -o shell shell.o pwd.o helpers.o cd.o help.o

# shell.o: shell.c pwd.h helpers.h cd.h help.h
# 	gcc -c shell.c -Wall -Werror

# pwd.o: pwd.c	
# 	gcc -c pwd.c

# cd.o: cd.c
# 	gcc -c cd.c

# help.o: help.c
# 	gcc -c help.c

# helpers.o: helpers.c	
# 	gcc -c helpers.c

# clean:
# 	rm -rf *.o shell

shell: shell.o helpers.o builtins.o extern_library.o
	gcc -o shell shell.o helpers.o builtins.o extern_library.o

shell.o: shell.c helpers.h builtins.h extern_library.h
	gcc -c shell.c -Wall -Werror

builtins.o: builtins.c	
	gcc -c builtins.c

helpers.o: helpers.c	
	gcc -c helpers.c

extern_library.o: extern_library.c
	gcc -c extern_library.c

clean:
	rm -rf *.o shell