# Simple-UNIX-Shell


A command-line shell for Linux/UNIX written in C, built directly on POSIX process and file-descriptor system calls (`fork`, `execvp`, `pipe`, `dup2`, `wait`) with no external libraries.

It supports multi-stage pipelines, input/output redirection, background execution and a few built-in commands, and it has a hand-written command parser.

## Features

| Feature | Syntax | Notes |
|---|---|---|
| Run programs | `ls -l /tmp` | Found through `PATH` using `execvp` |
| Pipelines | `cmd1 \| cmd2 \| cmd3` | Any number of stages |
| Input redirection | `sort < in.txt` | |
| Output redirection | `ls > out.txt`, `ls >> out.txt` | Truncate or append |
| Combined | `sort < in.txt \| uniq > out.txt` | Redirections work inside pipelines |
| Background jobs | `sleep 30 &` | Prompt returns immediately; finished jobs are reaped before the next prompt |
| Built-ins | `cd [dir]`, `exit`, `!!` | `cd` with no argument goes to `$HOME` |

## Getting started

**Requirements: gcc compiler, `make`, and a Linux/POSIX environment.**

```bash
git clone https://github.com/GoReef10/Simple-UNIX-Shell.git
cd Simple-UNIX-Shell
make run
```

| Command | Description |
|---|---|
| `make` | Build the shell into `bin/myshell` |
| `make run` | Build and start the shell |
| `make clean` | Remove the `obj/` and `bin/` directories |

The shell must be started from an interactive terminal. Type `exit` to quit.

## Example session

```text
[user@host ~/Simple-UNIX-Shell]: ls src | sort -r | head -n 2
shell.c
parser.c
[user@host ~/Simple-UNIX-Shell]: ls src > files.txt
[user@host ~/Simple-UNIX-Shell]: wc -l < files.txt
4
[user@host ~/Simple-UNIX-Shell]: echo done >> files.txt
[user@host ~/Simple-UNIX-Shell]: cat files.txt | wc -l
5
[user@host ~/Simple-UNIX-Shell]: !!
[user@host ~/Simple-UNIX-Shell]: cat files.txt | wc -l
[user@host ~/Simple-UNIX-Shell]: cd src
[user@host ~/Simple-UNIX-Shell/src]: exit
```

## How it works

```
input line --> parser() --> execute_builtins() --> execute_pipeline()
```
- **parser():** splits the line into tokens and builds a linked list of Command structs
- **execute_builtins():** handles cd / exit / !! in the parent process, so they affect the shell itself
- **execute_pipeline():** for each stage: pipe > fork > dup2 > execvp, then wait for the children

Each stage of a pipeline is stored as a `Command`, and stages are chained through the `next` pointer:

```c
typedef struct Command {
    char *args[COMMAND_NR];   // NULL-terminated argv for execvp system call
    int nr_args;
    char *infile;             // target of '<'
    char *outfile;            // target of '>' or '>>'
    int background;           // set by '&'
    int append;               // set when '>>' is present instead of '>'
    struct Command *next;     // next stage of the pipeline
} Command;
```

**Pipeline execution:** for each stage the shell creates a pipe, forks, and in the child redirects `stdin` from the previous stage's read end and `stdout` to the new pipe's write end with `dup2`. Explicit `<`, `>` and `>>` redirections are applied afterwards, so they take precedence over pipe connections. The parent closes every pipe end it no longer needs. The parent records the PID of every child it starts. A foreground job is waited on with `waitpid` for exactly those PIDs, while a background job (`&`) is left running. Finished background jobs are reaped with a non-blocking `waitpid(-1, NULL, WNOHANG)` before each prompt, so they never linger as zombie processes and never block later commands.

**Built-ins run in the parent:** `cd` has to run in the shell process itself, since a forked child would change only its own working directory.

### Project structure

```text
.
├── include/
│   └── shell.h             # Command struct, constants, function declarations
├── src/
│   ├── main.c              # Entry point
│   ├── shell.c             # Read-eval loop, prompt, path shortening
│   ├── parser.c            # Tokenizing and building the Command list
│   └── execute_commands.c  # Built-ins, process creation, pipes, redirection
├── makefile
└── README.md
```

## Limitations and future plans

This is a project motivated by learning and applying OS concepts and as a consequence some features are intentionally minimal for the time being:

- **Tokenizing:** operators must be separated by spaces (`ls | wc`, not `ls|wc`). There is no quoting or escaping, globbing, variable expansion, or `;` / `&&` operators.
- **Background jobs:** finished jobs are reaped when the next prompt is shown, not the instant they exit, and there is no job table (`jobs`, `fg`, `bg`). A `SIGCHLD` handler and basic job control are planned.
- **Signals:** there is no signal handling, so Ctrl-C terminates the shell rather than the foreground job.
- **History:** `!!` displays the previous command but does not re-run it. A full history feature is yet to be implemented
- **Interactive only:** input must come from a terminal. Reading commands from a script or pipe is not supported yet.

## Acknowledgements

Inspired in part by the UNIX shell project in **Operating System Concepts** by Abraham Silberschatz, Peter Galvin and Greg Gagne.