*This project has been created as part of the 42 curriculum by ymoubtas.*

# Get Next Line

## Description

**Get Next Line (GNL)** is a C programming project from the 42 curriculum. Its goal is to implement a function that reads from a file descriptor and returns one line at a time:

```c
char *get_next_line(int fd);
```

Each call returns the next available line. The returned string includes the terminating newline character (`\n`) when one is present. If the end of the file is reached and the final line has no newline, that last line is still returned without one. Once there is nothing left to read, or an error occurs, the function returns `NULL`.

The project focuses on reading incrementally, preserving data between function calls, managing dynamically allocated memory, and understanding the lifetime of **static variables**. It must work with regular files as well as standard input.

## Features

- Reads a file descriptor one line per function call.
- Handles lines shorter than, equal to, or longer than `BUFFER_SIZE`.
- Preserves unread characters for the next call instead of discarding data read beyond the current newline.
- Supports a final line that does not end with `\n`.
- Can be compiled with a custom `BUFFER_SIZE` or with the default defined by the project.
- The bonus implementation, when included, maintains separate reading state for multiple file descriptors so calls can be interleaved.

## Algorithm and design choices

The implementation uses **incremental reading with persistent leftover data**. Rather than reading the entire file into memory, it reads only as much as is needed to produce the next line.

### 1. Preserve data between calls

A persistent buffer (the saved or *held* data) keeps characters that have already been read from the descriptor but have not yet been returned to the caller. This is essential because a single `read()` can bring in more than one line. Returning the first line must not lose the characters after its newline.

The state is kept between calls with a static variable rather than a global variable. This allows a later call to continue where the previous call stopped.

### 2. Check the saved data first

At the beginning of a call, the implementation checks whether the saved data already contains a newline.

- If it does, the next line can be extracted without another system call. The newline is included in the returned line, and the remaining characters are kept for the next call.
- If it does not, the saved characters become part of the line being assembled, and more data must be read.

Checking the saved data first avoids unnecessary calls to `read()` when a complete line is already available.

### 3. Read only until a line is available

The function reads chunks of at most `BUFFER_SIZE` bytes and adds them to the line being assembled. It continues only while no newline has been found and no end-of-file or read error has occurred.

When a newline is found, the current line is returned and any characters after that newline are preserved as leftover data. The next call can therefore begin with those characters.

This approach was chosen because the function is expected to return one line per call. Reading the whole file up front would use unnecessary memory and would go against the subject's guidance to read as little as possible on each call.

### 4. Handle end-of-file and errors

At end-of-file:

- If characters have been accumulated, the function returns them as the final line, even if there is no terminating newline.
- If no characters remain, there is no line to return, so the function returns `NULL`.

If `read()` fails, the function returns `NULL` and temporary memory used for the unfinished line must be released. Saved state must also be handled so that stale data is not reused by a later call.

### Why this algorithm?

This design provides a balance between memory use and correct line handling:

- **Incremental:** it does not load the complete file into memory.
- **Stateful:** it remembers unread data across calls.
- **Buffer-size independent:** a line can span many reads, and one read can contain several lines.
- **Efficient for repeated calls:** already-buffered lines can be returned without another read.
- **Compatible with sequential reading:** it does not need `lseek()` to move around the file.

## Instructions

### Files

| File | Purpose |
|---|---|
| `get_next_line.c` | Main `get_next_line()` implementation |
| `get_next_line_utils.c` | Helper functions used by the implementation |
| `get_next_line.h` | Function prototype and required declarations |

The subject does not require a `main()` function. Add your own test driver when you want to build an executable.

### Compilation

Compile the mandatory files with the required compiler flags. For example, with a buffer size of 42:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   get_next_line.c get_next_line_utils.c main.c -o gnl
```

Replace `main.c` with your own test file. The `-D BUFFER_SIZE=42` option defines the number of bytes requested by each `read()` call.

The project must also compile without explicitly defining `BUFFER_SIZE`:

```sh
cc -Wall -Wextra -Werror \
   get_next_line.c get_next_line_utils.c main.c -o gnl
```

In that case, the default value defined by the project is used. The subject permits any default value.

### Example usage

The following example reads from standard input (`STDIN_FILENO`) and frees each returned line after using it:

```c
#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char *line;

    while ((line = get_next_line(STDIN_FILENO)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    return (0);
}
```

The caller is responsible for freeing each returned line.

### Testing recommendations

Test with different buffer sizes and input types. In particular, check:

- An empty file.
- A file containing only a newline.
- One line with a terminating newline.
- A final line without a terminating newline.
- Several consecutive lines.
- A line much longer than the buffer size.
- Several short lines that fit in one buffer read.
- `BUFFER_SIZE=1`, a value larger than the line length, and a large value such as `9999`.
- Reading from standard input as well as from a regular file.
- Repeated calls after the end of the file.
- For the bonus, interleaved calls using multiple file descriptors.

### Constraints and behavior defined by the subject

- The implementation may use `read()`, `malloc()`, and `free()` as its external functions.
- `lseek()` is forbidden.
- Global variables are forbidden.
- Using `libft` is forbidden.
- Reading binary files is undefined behavior for this project.
- Modifying the file after the last call, while `read()` has not yet reached the end of the file, results in undefined behavior.

## Bonus

The bonus requirements extend the function to support **multiple file descriptors at the same time**, while using only one static variable. Each descriptor must keep its own unread data. This makes interleaved calls possible—for example, reading from one descriptor, switching to another, and later returning to the first without mixing their lines or losing their positions.

| File | Purpose |
|---|---|
| `get_next_line_bonus.c` | Bonus implementation |
| `get_next_line_utils_bonus.c` | Bonus helper functions |
| `get_next_line_bonus.h` | Bonus declarations |

## Resources

- 42 Get Next Line subject, version 14.3 (the assignment specification for this project).
- [`read(2)` — Linux manual](https://man7.org/linux/man-pages/man2/read.2.html)
- [`malloc(3)` — Linux manual](https://man7.org/linux/man-pages/man3/malloc.3.html)
- [`free(3)` — Linux manual](https://man7.org/linux/man-pages/man3/free.3.html)
- [C storage duration — cppreference](https://en.cppreference.com/w/c/language/storage_duration)

### AI usage

AI tools were used as a learning aid to clarify concepts related to the project, including static variables, file descriptors, `read()` behavior, buffer sizes, and memory management. They were also used to discuss algorithmic decisions, examine edge cases, review reasoning, and help design tests.
