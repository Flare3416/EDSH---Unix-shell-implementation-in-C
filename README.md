# EDSH - A Simple Unix Shell Implementation in C

A lightweight Unix shell implementation written in C that provides basic command-line functionality with built-in commands and external program execution support.

## Features

### Built-in Commands

- **`echo`** - Print text to stdout or file
- **`cat`** - Display file contents
- **`pwd`** - Print current working directory
- **`cd`** - Change directory
- **`ls`** - List directory contents (with `-l` flag for detailed view)
- **`mkdir`** - Create new directory
- **`touch`** - Create new file
- **`rm`** - Remove file or directory
- **`mv`** - Move or rename files
- **`clear`** - Clear terminal screen
- **`exit`** - Exit the shell

### Additional Features

- **Output Redirection** - Redirect command output to files using `>` operator
- **External Program Execution** - Run any executable in the system PATH
- **Colorized Prompt** - Displays username, hostname, and current directory
- **Error Handling** - Comprehensive error messages with color-coded output

## Building the Project

### Prerequisites

- GCC compiler
- Unix/Linux environment (or WSL on Windows)
- Standard C library with POSIX headers

### Compilation

Simply run the `make` command in the project directory:

```bash
make
```

This will compile the source files and create the `edsh` executable.

### Clean Build

To remove compiled object files and executable:

```bash
make clean
```

## Usage

### Starting the Shell

```bash
./edsh
```

You'll see a prompt in the format:
```
username@hostname:/current/directory>
```

### Command Examples

```bash
# Print text
echo Hello World

# List files
ls
ls -l

# Navigate directories
cd /home
pwd

# Create files and directories
touch newfile.txt
mkdir newfolder

# Display file contents
cat newfile.txt

# Redirect output to file
echo "Hello" > output.txt
ls > filelist.txt

# Move or rename files
mv oldname.txt newname.txt

# Remove files
rm newname.txt

# Run external programs
gcc --version
python3 script.py

# Exit shell
exit
```

## Project Structure

```
.
├── command.c      # Implementation of built-in commands
├── command.h      # Header file with includes and function declarations
├── edsh.c         # Main shell loop and input processing
├── Makefile       # Build configuration
└── README.md      # This file
```

## Implementation Details

### File Descriptions

- **`command.c`** - Contains all built-in command implementations, argument parsing, output redirection handling, and external program execution logic
- **`command.h`** - Header file with system includes, constants, and function prototypes
- **`edsh.c`** - Main program entry point with command prompt display and input processing loop
- **`Makefile`** - Automated build configuration using GCC

### Key Functions

- `executeCommands()` - Main command dispatcher
- `collect_args()` - Parse command line into arguments
- `handle_redirection()` - Process output redirection operators
- `prog_exec()` - Execute external programs using fork/exec

## Technical Notes

- Uses `fork()` and `execvp()` for external program execution
- Implements ANSI escape codes for colored terminal output
- Maximum 20 arguments per command (configurable via `ARGMAX`)
- Input buffer size: 256 characters
- Directory path buffer: 256 characters

## Limitations

- No pipe (`|`) support
- No background process execution (`&`)
- No input redirection (`<`)
- No command history
- No tab completion
- Single command per line only

## Error Handling

The shell provides informative error messages for:
- Invalid commands
- Missing arguments
- File access errors
- Directory navigation errors
- Fork/exec failures

Error messages are displayed in red using ANSI color codes.

## License

This project is provided as-is for educational purposes.

## Author

Created as a shell scripting project demonstrating Unix system calls and process management in C.
