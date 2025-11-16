#include "command.h"

#define ARGMAX 20

char *arguments[ARGMAX];
FILE *out_fh;
int argnum = 0;

void initialize_args()
{
    for (int i = 0; i < ARGMAX; i++)
    {
        arguments[i] = NULL;
    }
    argnum = 0;
}

void collect_args(char *command)
{
    arguments[0] = strtok(command, " \n\r\t");
    argnum = 1;
    for (int i = 1; i < ARGMAX; i++)
    {
        arguments[i] = strtok(NULL, " \n\r\t");
        if (arguments[i] == NULL)
        {
            break;
        }
        argnum++;
    }
}

FILE *handle_redirection()
{
    int i;
    for (i = 1; i < argnum; i++)
    {
        if (strcmp(arguments[i], ">") == 0)
        {
            if (i == argnum - 1)
            {
                fprintf(stderr, "\033[1;31mParse error: No file specified for redirection.\n\033[0m");
                return NULL;
            }

            char *filedest = arguments[i + 1];
            FILE *file = fopen(filedest, "w");
            if (file == NULL)
            {
                fprintf(stderr, "\033[1;31mError opening file %s: %s\n\033[0m", filedest, strerror(errno));
            }
            return file;
        }
    }
    return stdout;
}

void edsh_echo()
{
    for (int i = 1; i < argnum; i++)
    {
        if (arguments[i] == NULL || strcmp(arguments[i], ">") == 0)
            break;
        fprintf(out_fh, "%s", arguments[i]);

        if (i < argnum - 1)
            fprintf(out_fh, " ");
    }

    fprintf(out_fh, "\n");
}

void edsh_cat() 
{
    if(argnum == 1) 
    {
        fprintf(stderr, "\033[1;31mMissing filename.\n\033[0m");
        return;
    }
    
    char* filename = arguments[1];
    if (filename)
    {
        FILE* file = fopen(filename, "r");
        if (file == NULL)
        {
            fprintf(stderr, "\033[1;31mError opening file %s: %s\n\033[0m", filename, strerror(errno));
            return;
        }
        int c;
        while ((c = fgetc(file)) != EOF) 
        {
            fputc(c, out_fh);
        }
        fclose(file);
    }
} 
void edsh_pwd() 
{
    char temp[DIRECTORY_SIZE];
    if (getcwd(temp, DIRECTORY_SIZE) == NULL) 
    {
        fprintf(stderr, "Failed to get current working directory");
    } 
    else if (fprintf(out_fh, "%s\n", temp) < 0)
    {
        fprintf(stderr, "Failed to write to destination");
    }
}

void edsh_cd() 
{
    int n;
    if (argnum == 1) 
    {
        n = chdir("/usercode");
    } 
    else
    {
        n = chdir(arguments[1]);
    }
    if (n == -1) 
    {
        fprintf(stderr, "\033[1;31mError changing directory: %s\n\033[0m", strerror(errno));
    }
}
void edsh_ls()
{
    int n;
    struct dirent **directory_list;
    char *dirpath = ".";
    int showDetails = 0; // Flag to indicate whether to show additional file details
    // Parse arguments for directory
    for (int i = 1; i < argnum; i++) 
    {
        if (arguments[i] == NULL || strcmp(arguments[i], ">") == 0) 
        {
            break;
        } 
        else if(strcmp(arguments[i], "-l") == 0)
        {
            showDetails = 1;
        } 
        else if(strcmp(dirpath, ".") == 0)
        {
                dirpath = arguments[i];
        }
    }
    // Scan the directory
    n = scandir(dirpath, &directory_list, NULL, alphasort);
    if (n == -1) 
    {
        fprintf(stderr, "\033[1;31mError scanning directory: %s\n\033[0m", strerror(errno));
        return;
    }

    if (showDetails)
    {
        fprintf(out_fh, "Permissions\tSize\tLast Modified\t\tFile Name\n");
        fprintf(out_fh, "-----------\t----\t-------------------\t------------\n");
    }

    for (int i = 0; i < n; i++) 
    {
        if (showDetails)
        {
            struct stat fileStat;
            char filePath[2*DIRECTORY_SIZE];
            snprintf(filePath, sizeof(filePath), "%s/%s", dirpath, directory_list[i]->d_name);

            if (stat(filePath, &fileStat) == 0) 
            {
                fprintf(out_fh, (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
                fprintf(out_fh, (fileStat.st_mode & S_IRUSR) ? "r" : "-");
                fprintf(out_fh, (fileStat.st_mode & S_IWUSR) ? "w" : "-");
                fprintf(out_fh, (fileStat.st_mode & S_IXUSR) ? "x" : "-");
                fprintf(out_fh, (fileStat.st_mode & S_IRGRP) ? "r" : "-");
                fprintf(out_fh, (fileStat.st_mode & S_IWGRP) ? "w" : "-");
                fprintf(out_fh, (fileStat.st_mode & S_IXGRP) ? "x" : "-");
                fprintf(out_fh, (fileStat.st_mode & S_IROTH) ? "r" : "-");
                fprintf(out_fh, (fileStat.st_mode & S_IWOTH) ? "w" : "-");
                fprintf(out_fh, (fileStat.st_mode & S_IXOTH) ? "x" : "-");

                char datetime[20];
                strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", localtime(&fileStat.st_mtime));
                fprintf(out_fh, "\t%ld\t%s\t%s\n", fileStat.st_size, datetime, directory_list[i]->d_name);
            } 
            else
            {
                fprintf(stderr, "Error getting file details for %s: %s\n", directory_list[i]->d_name, strerror(errno));
            }
        }
        else
        {
            fprintf(out_fh, "%s\n", directory_list[i]->d_name);
        }
        free(directory_list[i]); // Free each dirent structure
    }
    free(directory_list); // Free the array of pointers
}

void edsh_touch() 
{
    if(argnum == 1) 
    {
        fprintf(stderr, "\033[1;31mMissing filename.\n\033[0m");
        return;
    }
    char *filename = arguments[1];
    if (filename) 
    {
        // check if the file exists
        if (access(filename, F_OK) == -1) 
        {
            // create the file
            FILE *file = fopen(filename, "w");
            if (file != NULL) 
            {
                fclose(file);
            } else 
            {
                perror("Error creating file");
            }
        }
    }
}

void edsh_mkdir() 
{
    if(argnum == 1)
    {
        fprintf(stderr, "\033[1;31mMissing directory name.\n\033[0m");
        return;
    }
    char *dirname = arguments[1];
    if (dirname) 
    {
        if (mkdir(dirname, 0777) != 0)
        {
            perror("Error creating directory");
        }
    }
}
void edsh_rm() 
{
    if(argnum == 1)
    {
        fprintf(stderr, "\033[1;31mMissing filename.\n\033[0m");
        return;
    }
    char *filename = arguments[1];
    if (filename) 
    {
        if (remove(filename) != 0) 
        {
            perror("Error in deletion");
        }
    }
}
void edsh_mv()
{
    if(argnum <= 2) 
    {
        fprintf(stderr, "\033[1;31mMissing source or destination.\n\033[0m");
        return;
    }
    char *source = arguments[1];
    char *destination = arguments[2];
    if (source && destination) 
    {
        if (rename(source, destination) != 0) 
        {
            perror("Error moving file");
        }
    }
}

void prog_exec() 
{
    pid_t pid_child = fork();
    if (pid_child == 0) 
    {
        // Child process
        if (execvp(arguments[0], arguments) == -1)
        {
            fprintf(stderr, "\033[1;31m%s\n\033[0m", strerror(errno));
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    } 
    else if (pid_child > 0)
    {
        // parent process
        if (wait(NULL) == -1)
        {
            perror("wait error");
        }
    } 
    else 
    {
        // fork failed
        perror("fork error");
        exit(EXIT_FAILURE);
    }
}

void executeCommands(char *command)
{
    initialize_args();
    collect_args(command);
    
    if (arguments[0] == NULL)
        return; // empty command
    
    out_fh = handle_redirection();
    if (out_fh == NULL)
        return; // error already printed

    if (strcmp(arguments[0], "exit") == 0)
    {
        fprintf(stdout, "Closing edsh\033[5m...\033[0m\n");
        sleep(1);
        exit(0);
    }
    else if (strcmp(arguments[0], "clear") == 0)
    {
        printf("\033[2J\033[1;1H");
    }
    else if (strcmp(arguments[0], "echo") == 0)
    {
        edsh_echo();
    }
    else if (strcmp(arguments[0], "cat") == 0)
    {
        edsh_cat();
    }
    else if (strcmp(arguments[0], "pwd") == 0)
    {
        edsh_pwd();
    }
    else if (strcmp(arguments[0], "cd") == 0)
    {
        edsh_cd();
    }
    else if(strcmp(arguments[0], "ls") == 0) 
    {	
        edsh_ls();
    }
    else if (strcmp(arguments[0], "mkdir") == 0) 
    {
        edsh_mkdir();
    } 
    else if (strcmp(arguments[0], "touch") == 0) 
    {
        edsh_touch();
    }
    else if (strcmp(arguments[0], "rm") == 0) 
    {
        edsh_rm();
    } 
    else if (strcmp(arguments[0], "mv") == 0) 
    {
        edsh_mv();
    }
    else if (arguments[0])
    {
       prog_exec();
    }
    else
    {
        printf("\033[1;31mInvalid Command\n\033[0m");
    }

    if (out_fh != stdout)
    {
        fclose(out_fh);
    }
}