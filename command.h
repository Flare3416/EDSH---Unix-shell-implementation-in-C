#ifndef COMMAND_H
#define COMMAND_H
#define INPUT_SIZE 256
#define HOSTNAME_SIZE 64
#define USERNAME_SIZE 32
#define DIRECTORY_SIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

// Add preprocessor directives here

void executeCommands(char *command);

#endif