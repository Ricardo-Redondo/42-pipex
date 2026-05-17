# pipex
 
A 42 school project that recreates the `|` (pipe) operator from Bash through processes, file descriptors, pipes, and command execution.
 
---
 
## Table of Contents
 
- [Description](#description)
- [Project Structure](#project-structure)
- [Compilation](#compilation)
- [Mandatory Part](#mandatory-part)
- [Bonus Part](#bonus-part)
- [Key Concepts](#key-concepts)
- [Testing & Debugging](#testing--debugging)
- [Error Handling](#error-handling)
---
 
## Description
 
`pipex` recreates shell piping in C. Given an input file, two (or more) commands, and an output file, it pipes the output of one command into the next:
 
```bash
< file1 cmd1 | cmd2 > file2
```
 
The project requires:
- **Process management** — `fork`, `wait`, `waitpid`
- **Program execution** — `execve`
- **File descriptors** — `dup2`, `open`, `close`
- **Inter-process communication** — `pipe`
- **Environment variables** — `envp`, `PATH`
 
## Project Structure
 
```
pipex
├── includes
│   ├── pipex.h
│   └── pipex_bonus.h
├── libft
├── srcs
│   ├── pipex.c
│   ├── utils.c
│   ├── pipex_bonus.c
│   └── utils_bonus.c
├── LICENSE
├── Makefile
├── pipex_subject.pdf
└── README.md
```

 
## Compilation
 
| Command | Description |
|---------|-------------|
| `make` | Compile mandatory |
| `make bonus` | Compile bonus version |
| `make clean` | Remove object files |
| `make fclean` | Remove objects and executable |
| `make re` | Rebuild mandatory |
| `make rebonus` | Rebuild bonus |
| `make create` | Create test input/output files |
| `make delete` | Delete test input/output files |

 
## Mandatory Part
 
### Usage
 
```bash
./pipex infile "cmd1" "cmd2" outfile
```
 
Equivalent to:
```bash
< infile cmd1 | cmd2 > outfile
```
 
### How It Works
 
**1. Create a pipe**
 
Using `pipe(fd)` creates a channel between two processes, giving us two file descriptors:
 
```c
fd[0] → read end
fd[1] → write end
```
 
**2. Fork the process**
 
`fork()` creates a child process. Now we have two processes running in parallel.
 
#### Child Process
- Opens `infile` for reading
- Redirects stdin to infile using `dup2(filein, STDIN_FILENO)`
- Redirects stdout to the pipe's write end using `dup2(fd[1], STDOUT_FILENO)`
- Executes `cmd1` via `execute()`, which finds the command in **PATH** and calls `execve`
- **Result:** `cmd1` reads from `infile` and writes to the pipe
#### Parent Process
- Waits for the child to finish with `waitpid()`
- Opens `outfile` for writing (truncate mode)
- Redirects stdout to outfile using `dup2(fileout, STDOUT_FILENO)`
- Redirects stdin from the pipe's read end using `dup2(fd[0], STDIN_FILENO)`
- Executes `cmd2` via `execute()`, replacing the parent process
- **Result:** `cmd2` reads from the pipe and writes to `outfile`
### Key Functions
 
- **`find_path()`** — Locates the **PATH** variable in `envp`, splits it by `:`, and searches each directory for the command using `access()`.
- **`execute()`** — Parses the command string into arguments using `ft_split`, finds the full path, and calls `execve` to run the program.
- **`error()`** — Centralized error handling using `perror`.
### Data Flow
 
```
infile → [cmd1 (child)] → pipe → [cmd2 (parent)] → outfile
```

 
## Bonus Part
 
The bonus part extends pipex with two additional features:
 
### 1. Multiple Pipes
 
Handles any number of commands chained together:
 
```bash
./pipex infile cmd1 cmd2 cmd3 ... cmdn outfile
```
 
Equivalent to:
```bash
< infile cmd1 | cmd2 | cmd3 | ... | cmdn > outfile
```
 
#### How It Works
 
- **`init_fds(argc)`** — Dynamically allocates and creates `N-1` pipes for `N` commands, stored in a NULL-terminated 2D array (`int **fds`).
- **`multiple_pipes()`** — Forks the first command (reads from `infile`, writes to `fds[0]`).
- **`child_proccess()`** — Loops through the middle commands, forking each one. Each middle command reads from the previous pipe (`fds[i-1][0]`) and writes to the next pipe (`fds[i][1]`).
- **`parent_proccess()`** — Executes the last command in the parent process itself; it reads from the last pipe (`fds[argc-5][0]`) and writes to `outfile` (truncate mode).
#### Data Flow
 
```
infile → cmd1 → fds[0] → cmd2 → fds[1] → cmd3 → ... → fds[n-2] → cmdn → outfile
```
 
---
 
### 2. Heredoc
 
Reads input from stdin until a user-defined **LIMITER** is reached, then pipes it through two commands, **appending** to the output file:
 
```bash
./pipex here_doc LIMITER cmd1 cmd2 outfile
```
 
Equivalent to:
```bash
cmd1 << LIMITER | cmd2 >> outfile
```
 
#### How It Works
 
**`hd_child_proccess()`:**
- Reads lines from stdin using `get_next_line()`
- Joins the limiter with `\n` to handle newline-terminated lines
- Compares each line with the delimiter — breaks when matched
- Writes each line to the first pipe (`fds[0][1]`)
- Forks an inner process to execute `cmd1`, which reads from `fds[0][0]` and writes to `fds[1][1]`
**`hd_parent_proccess()`:**
- Opens `outfile` in **APPEND mode** (`O_APPEND`) — heredoc appends, doesn't overwrite
- Redirects stdin from `fds[1][0]` and stdout to `outfile`
- Executes `cmd2`
#### Data Flow
 
```
stdin (until LIMITER) → fds[0] → cmd1 → fds[1] → cmd2 → outfile (append)
```
 
---
 
### Detection Logic
 
The bonus `main()` detects which mode to use:
 
```c
if (argv[1] equals "here_doc")
    heredoc mode
else
    multiple pipes mode
```

 
## Key Concepts
 
### File Descriptors
 
```
0 = STDIN_FILENO
1 = STDOUT_FILENO
2 = STDERR_FILENO
3+ = pipes, opened files
```
 
### dup2(old_fd, new_fd)
 
Makes `new_fd` point to the same file as `old_fd`. This is how we redirect input/output:
 
```c
dup2(pipefd[1], STDOUT_FILENO);  // write to pipefd[1] instead of stdout
dup2(pipefd[0], STDIN_FILENO);   // read from pipefd[0] instead of stdin
```
 
### Pipe
 
Data written to `pipefd[1]` can be read from `pipefd[0]`. This is done **before** forking so both child and parent can access the pipe.
 
```c
int pipefd[2];
pipe(pipefd);
// pipefd[0] = read from index 0
// pipefd[1] = write to index 1
```
 
### Fork
 
`fork()` creates a copy of the current process (child) that runs at the same time as the parent. It returns:
 
```c
pid_t pid = fork();
// pid == 0    → I'm the child
// pid > 0     → I'm the parent (pid = child's PID)
// pid == -1   → fork failed
```
 
### Execve
 
`execve` requires the full path to the command, the command's arguments, and the environment variable table. This project uses `execve` instead of `execvp` for learning purposes (manual PATH searching).
 
```c
execve(path, args, envp);
// Replaces current process with new program
// Only returns on failure
```
 
### Finding Commands in PATH
 
The `find_path()` function:
 
1. Locates `PATH=` in `envp`
2. Splits PATH by `:` into directories
3. For each directory, checks if `dir/command` exists and is executable
4. Returns the full path or `NULL`
```
PATH=/usr/local/bin:/usr/bin:/bin
   ↓ split by ':'
["/usr/local/bin", "/usr/bin", "/bin"]
   ↓ append "/ls"
Try: /usr/local/bin/ls, /usr/bin/ls, /bin/ls
```
 
 
## Testing & Debugging
 
### Memory Leaks with Valgrind
 
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./pipex file_in.txt "cat" "grep Hello" file_out.txt
```
 
**Note:** Leaks inside `cat`, `grep`, and other external programs are from those programs themselves, not your code.
 
 
## Error Handling
 
The program handles:
 
| Error | Response |
|-------|----------|
| Wrong number of arguments | Prints usage |
| Invalid input file | Error message via `perror` |
| Failed pipe/fork | Error message via `perror` |
| Command not found | Error message |
| Failed execve | Error message |
