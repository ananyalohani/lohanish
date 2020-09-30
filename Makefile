TARGETS = external/rm_cmd external/mkdir_cmd external/ls_cmd external/cat_cmd external/date_cmd
CC = gcc
CFLAGS = -w
OBJ = shell
MAIN = main

#run make to generate the binary for the shell
#run the shell program with ./shell
all: clean $(OBJ)

$(OBJ): $(TARGETS)
	$(CC) $(CFLAGS) $(MAIN).c -o $@

$(TARGETS):
	$(CC) $(CFLAGS) $@.c -o $@

#run make clean to remove all the executables generated after compilation
clean:
	rm -f $(OBJ) $(TARGETS)