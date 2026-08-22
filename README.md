# lightshell
My lightweight UNIX shell written in C++, built for learning purposes.
 
![C++](https://img.shields.io/badge/language-C%2B%2B-blue)
![Status](https://img.shields.io/badge/status-work--in--progress-yellow)

## Why this project?
I've always wanted to understand how a UNIX shell actually works under the hood —
how it reads input, parses commands, spawns processes, and handles pipes and
redirections. Rather than just reading about it, I decided the best way to learn
is to build one myself, one feature at a time.
 
## Why C++?
C++ was the first language I learned to program in, but I never felt I truly
mastered it. This project is also a way to get more comfortable with it —
memory management, the standard library, and lower-level system programming
(syscalls, process handling, file descriptors) all in one place.

## Future TODO:
- [x] Execute real programs
- [x] Use colors when there are problems
- [x] Understand flags
- [x] Persistent command history (saved to `~/.lsh/history.txt`)
- [ ] Pipes
- [ ] I/O redirection
- [ ] Quoting/escaping support in the tokenizer (e.g. `"file with spaces"`)
- [ ] Environment variable expansion (`$HOME`) and tilde expansion (`~`)
- [ ] Command chaining (`&&`, `||`, `;`)
- [ ] Background execution (`&`) and basic job control
- [ ] Signal handling
- [x] Create make file with optimizations

## Requirements
- A POSIX/Linux system (the shell relies on `fork`, `execvp`, `waitpid`)
- A C++ compiler with C++17 support or later (needed for `<filesystem>`)
- `make`

## Building
```bash
git clone https://github.com/Kerlooo/lightshell.git
cd lightshell
make
```

This builds an optimized release binary (`-O2 -s`). For a debug build with
sanitizers enabled, run `make debug` instead — it produces `lightshell-debug`.

## Usage
```bash
./lightshell
```

## Sources & AI usage
This project is a learning exercise, so I try to be transparent about where the
knowledge behind it comes from:

- **YouTube tutorials** on writing a shell (mostly in C or Rust) for the general
  architecture and step-by-step logic
- **StackOverflow** for understanding specific functions and system calls
- **Claude (Anthropic)** used only as a study aid — to understand *how* things
  work and *why*, and to get guidance on writing the shell more securely (e.g.
  avoiding unsafe patterns around process handling, input parsing, memory
  management). It is not used to generate the shell's code for me; the
  implementation is my own.

## License
This project is distributed under the Creative Commons Attribution 4.0 International License (CC BY 4.0).

You are free to:

Share — copy and redistribute the material in any medium or format.
Adapt — remix, transform, and build upon the material for any purpose, even commercially.
Under the following terms:

Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made.
For more details, see the [LICENSE](LICENSE) file or visit [creativecommons.org](creativecommons.org).