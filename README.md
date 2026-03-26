# myshell 🐚

A Unix shell built from scratch in C, running on macOS.

## Features
- **Command execution** — run any system command (`ls`, `pwd`, `echo`, etc.)
- **cd & exit** — built-in navigation and exit
- **Pipes** — chain commands together (`ls | grep .c`)
- **Output redirection** — save output to files (`ls > output.txt`)
- **Colored prompt** — green and blue styled prompt
- **Error handling** — friendly messages for unknown commands

## Build & Run
```bash
git clone https://github.com/Baranidharanv06/myshell.git
cd myshell
gcc shell.c -o myshell -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include -lreadline
./myshell
```

## Usage Examples
```bash
myshell> ls
myshell> pwd
myshell> echo hello world
myshell> cd ..
myshell> ls | grep .c
myshell> ls > output.txt
myshell> cat output.txt
myshell> exit
```

## What I learned
- How a Unix shell works under the hood
- Process creation using `fork()` and `execvp()`
- Inter-process communication using `pipe()`
- File descriptors and I/O redirection with `dup2()`
- Parsing and tokenizing user input in C

## Tech Stack
- Language: C
- Platform: macOS (Apple Silicon)
- Library: readline