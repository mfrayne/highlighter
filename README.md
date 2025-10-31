# highlighter Programming Assignment

In this assignment, you are going to implement your own CLI program called `highlighter` which utilizes many aspects of IO in C and Linux/Unix. You'll get experience using file descriptors or streams, different IO patterns, string functions, error handling, and argument parsing.

## Accepting the Assignment

[Click here to accept this assignment](https://classroom.github.com/a/XaEz5NDb). Once you accept the invitation, GitHub will create a private repository containing the starter files for the assignment. You will also find an accompanying `README.md`, which is essentially this document you are reading, useful for instructions and further details.


## Details

You will be implementing an application called `highlighter` which has the following help menu:

```bash
$ ./highlighter --help
usage: ./highlighter [-h] [-i INPUT] [-o OUTPUT] [-c {RED,GREEN,BLUE,YELLOW,MAGENTA,CYAN,WHITE}] [--no-color] [--prefix PREFIX] [--postfix POSTFIX] text

highlighter

positional arguments:
  text                  text to match and highlight

options:
  -h, --help            show this help message and exit
  -i INPUT, --input INPUT
                        input file (default: stdin)
  -o OUTPUT, --output OUTPUT
                        output file (default: stdout)
  -c {RED,GREEN,BLUE,YELLOW,MAGENTA,CYAN,WHITE}, --color {RED,GREEN,BLUE,YELLOW,MAGENTA,CYAN,WHITE}
                        color option (default: RED)
  --no-color            do not print color
  --prefix PREFIX       print prefix before highlighted text (default: empty string)
  --postfix POSTFIX     print postfix after highlighted text (default: empty string)

examples:
  ./highlighter -i file.txt "error" (highlights "error" in file.txt in red via STDOUT)
  ./highlighter -c GREEN "warning" (highlights "warning" in green from STDIN to STDOUT)
  ./highlighter --prefix ">>" --postfix "<<" "keyword" -o out.txt (highlights "keyword" with prefix ">>" and postfix "<<" in red from STDIN to out.txt)
```

### Program Requirements

Your `highlighter` program should:

1. **Parse arguments**: Accept the positional argument `text` and optional flags for input, output, color, and formatting options
   - If there are any errors paring arguments, opening files, or malformed inputs, you should print to `STDERR` with the text `Error: <your message>` and exit main with a non-zero exit code.
2. **Read input**: Read from stdin (default) or from a file specified with `-i`
   - You will be reading line by line. The highlighter will not highlight across new line boundaries.
3. **Find matches**: Search for all occurrences of the specified text in the input
   - A match is when the search text matches any non-overlapping substrings of the input line.
4. **Highlight matches**: Output matched text with the specified color (using ANSI escape codes provided in `highlighter.c`) and apply any additional prefix and postfix to matches, if specified. The color is not applied to the prefix/postfix.
6. **Write output**: Write to stdout (default) or to a file specified with `-o`.
   - Do not write to STDOUT if a file is supplied. You will only write to one location.
8. **Display Statistics**: When your program terminates, you should print to `STDERR` the number of matches formatted as follows: `Highlighted %d matches.\n`. Return from main with a 0 exit code.

### Argument Details

- **-h, --help**: Display the help message and exit (can be anything, or copied from above).
- **-i, --input**: Path to input file (if not specified, read from stdin) (if the flag is specified, a filename must follow)
- **-o, --output**: Path to output file (if not specified, write to stdout) (if the flag is specified, a filename must follow) (always overwrite the file with new output)
- **-c, --color**: Color to use for highlighting (RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, or WHITE) (default is RED) (if the flag is specified, a valid color must follow)
- **--no-color**: Disable color highlighting
- **--prefix**: Text to print before each highlighted match (the next argument shall be the prefix) (the prefix should not be colored)
- **--postfix**: Text to print after each highlighted match (the next argument shall be the postfix) (the postfix should not be colored)
- **text** (positional): The text to search for and highlight (if no flags/options are consumed/parsed you may assume that the argument is the highlight text input; i.e. it is position independent) (if there are multiple positional arguments, error out saying there can only be one)

### ANSI Color Codes

The `highlighter.c` file already includes ANSI escape codes for colors. To highlight text, print/write:
1. The color code (e.g., `COLOR_RED`)
2. The text to highlight
3. The reset code (`COLOR_RESET_COLOR`)

Example:
```c
fprintf(stream, "%s%s%s", COLOR_RED, matched_text, COLOR_RESET_COLOR);
```

### Implementation Details

You may use file descriptors (using Linux system calls) or you can use file streams (using Linux system libraries). You may use any of the following functions in your design (but this is not a limited set):
- syscalls (file descriptors)
  - open/close
  - read/write
- streams (FILE* streams)
  - fopen/fclose
  - fread/fwrite
  - fgetc/fgets/fputc/fputs
  - getline (recommended for reading lines dynamically)
  - scanf
  - printf/fprintf
- string manipulation/interpretation/parsing
  - strcmp/strncmp
  - strcpy/strncpy
  - strstr
  - sscanf
  - atoi/f/l (ascii to integer/float/long)
  - strtol/d (string to long/double)
  - strtok

Feel free to look at the man pages for any of these functions to inspect their function signature and use case.

#### Recommended Implementation Approach

A typical implementation approach includes:

1. **Argument Parsing**: Parse command-line arguments to extract:
   - Input file path (default: stdin)
   - Output file path (default: stdout)
   - Color option (default: RED)
   - Prefix and postfix strings
   - Flags (`--help`, `--no-color`)
   - Else, assume it's the positional argument (search text)

2. **File Handling**: 
   - Open a handle/stream for the input origin; if `-i` is specified use the file, otherwise use `stdin`
   - Open a handle/stream for the output destination; if `-o` is specified use the path, otherwise use `stdout`

3. **Line-by-Line Processing**:
   - Use `read`-like commands to read lines from the input stream
   - Process each line to find **non-overlapping** matches of the search text
   - For each match, output: prefix (if any) → color code → match text → reset code → postfix (if any)
   - Track the total number of matches across all lines

4. **Error Handling**:
   - All error messages must start with `Error: ` and be printed to `stderr`
   - Common errors include: missing arguments, invalid colors, file open failures, empty search text, multiple positional arguments

5. **Output Statistics**:
   - After processing all lines, print to `stderr`: `Highlighted %d matches.\n`
   - This should be the final output to stderr before program termination

6. **Memory Management**:
   - Ensure proper memory safety and no memory leaks (valgrind will check this)

## Assignment File Structure

### **highlighter.c**

**This is the only file you should be modifying in this assignment.**

This file is a *source* file written in C (due to the `.c` file extension). This file contains a starter implementation with the main function and color code definitions. Inside, you will write your own C code to implement the highlighter program based on the requirements above. Additionally, we would like you to adhere to good programming practices; i.e. include comments while writing your code. This will help you understand your code long after writing it and will also help others, such as course staff, to be able to read your code and understand your thought process. To encourage the use of comments, a portion of this assignment will be based on whether you have written effective comments (telling us what you are doing in the code, why you write the code in this way, etc.).

For example, consider these two samples of code:

```c
// f to c
float convert_to_c(float input) {
    float c = 0; // declare var
    float f = input; // declare input var
    c = (f - 32) * 5 / 9; // do math
    return c; // return result
}
```

```c
// Convert an input float in Fahrenheit (f) to an output float in Celsius (c)
float convert_to_c(float input) {
    float c = 0; // declare the output celsius float
    float f = input; // declare the input fahrenheit float, set it as the input
    c = (f - 32) * 5 / 9; // convert F to C using the conversion formula: C = (F - 32) × 5/9
    return c; // return the celcius result
}
```

While both functions do the same thing, they are documented very differently. The latter is much more readable and understandable as to why decisions were made and what the code is doing rather than just describing the code on a surface level. When writing comments, it's not enough to just say "this is what the code does" but instead you should be documenting "why the code is written as it is". Comments should explain your thought process.

### **tests/**

This directory contains test inputs and expected outputs for grading:

- **tests/inputs/**: Contains various test input files including:
  - `simple.txt`, `test.txt` - Basic text files for simple highlighting tests
  - `empty.txt` - Empty file to test edge cases
  - `newlines.txt` - File with multiple newlines
  - `multiline.txt` - Multi-line text for testing line-by-line processing
  - `verylong.txt`, `lorem_upsum_long.txt` - Longer files for comprehensive testing
  - `aaa.txt` - File with repeating patterns
  - `simple_story.txt` - Story text for realistic testing scenarios
  
- **tests/outputs/**: Contains expected output files organized as follows:
  - `*.stdout` files - Expected standard output for tests writing to stdout
  - `*.txt` files - Expected file outputs for tests writing to output files
  - `error.txt` - Expected error message format (first 7 characters: "Error: ")
  - **matches/** subdirectory - Contains expected match count outputs (files named by match count: `0.txt`, `1.txt`, `2.txt`, etc.)
    - Each match file contains: `Highlighted N matches.\n` where N is the number of matches
  
- **tests/tests.txt**: Contains all test commands organized by sections (invalid inputs, help inputs, actual inputs, valgrind inputs). Each section has a percentage weight for grading.

### **grade.py**

You don't need to read this file to finish this assignment, but it's a Python script that runs tests on your compiled program to check for correctness. The script:
- Reads test commands from `tests/tests.txt`
- Executes each test command with a timeout
- Compares stdout/stderr outputs with expected outputs in `tests/outputs/`
- Calculates your score based on test sections (invalid inputs: 15%, help inputs: 5%, actual inputs: 65%, valgrind: 15%)
- The correctness portion is worth 23 points of your grade (out of 25 total)

### **.gitignore**

You don't need to read this file to finish this assignment. 

This file tells git what files or folders it should ignore. Any changes in those files or folders will not be tracked. The content of this file is `highlighter` executable and the valgrind outputs.

### **README.md**

The markdown document you are reading now.

---

## Testing Your Code

When you finish a function or reach a stopping point, you can compile and test your code using the following commands.

### Compiling

In the project directory, run this command in your terminal to compile your code:

```shell
gcc highlighter.c -o highlighter
```

If your code has no syntax errors, an executable file called `highlighter` will appear in your directory. If there are syntax errors, carefully read the errors or warnings to understand what line of code is causing the syntax error.

### Testing / Running

You can test your program yourself with any input you choose:

```shell
# Basic usage
./highlighter "error"

# Specify input file
./highlighter -i file.txt "error"

# Specify color
./highlighter -i file.txt "error" -c MAGENTA

# With prefix and postfix
./highlighter -i file.txt "error" --prefix ">>>" --postfix "<<<"

# Output to file
./highlighter -i file.txt "error" -o output.txt

# No color (for piping to files)
./highlighter -i file.txt "error" --no-color
```

Do this as many times as you'd like to ensure you are getting proper outputs. You can extract one command out of `tests/tests.txt` to test one of those manually. The end of each test returns 0 (success) or non-zero (failure). You can append the following at the end of each of those to see if you'd pass each or not: `<test> && echo "Pass" || echo "Failed"`.

### Running the Test Suite

To test your code and see your proposed grade, run:

```shell
python3 grade.py
```

or 

```shell
chmod +x grade.py
./grade.py
```

This script will:
- Check for the `highlighter` executable in the current directory
- Read test commands from `tests/tests.txt`
- Organize tests by sections with percentage weights:
  - Invalid inputs (15%): Tests for malformed arguments, missing files, invalid colors
  - Help inputs (5%): Tests for `-h`/`--help` flag behavior
  - Actual inputs (65%): Functional tests covering all features and edge cases
  - Valgrind inputs (15%): Memory safety tests (last 4 tests)
- Execute each command
- Compare stdout and stderr with expected outputs in `tests/outputs/`
- Verify that output files are created/not created as expected
- Display detailed test results with pass/fail status
  - If things pass, no output is generated. If things fail you will see the diff outputted.
- Calculate and display your score (out of 23 points for correctness)

The script assumes there is an executable called `highlighter` in your current working directory. Make sure you've compiled your program first using `gcc highlighter.c -o highlighter`.

#### Valgrind Testing

The last 4 tests in `tests/tests.txt` run valgrind to check for memory leaks and errors:
- Tests run with `valgrind`
- Valgrind output is stored in a file: `valgrind_output1.log` through `valgrind_output4.log`
- Tests must pass both correctness checks AND valgrind memory checks
- You can inspect the valgrind logs if a test fails to identify memory issues

---

## Rubric

This assignment is worth 25 points total.

- **Output/Diff Correctness (23 pts)**: Your program must pass all test cases in the grading script. The grade is calculated based on test sections with different weights:
  - **Invalid inputs (15% = ~3.45 pts)**: Tests for error handling with malformed arguments:
    - Missing positional argument (search text)
    - Multiple positional arguments
    - Missing file arguments after `-i`, `-o`, `-c`, `--prefix`, `--postfix`
    - Invalid color names
    - Non-existent input files
    - Empty search text
    - All errors must print `Error: ` prefix to stderr
  - **Help inputs (5% = ~1.15 pts)**: Tests for `-h`/`--help` flag:
    - Help message printed to stdout (not stderr)
    - No output files created even if specified with help flag
  - **Actual inputs (65% = ~14.95 pts)**: Functional tests covering:
    - Basic highlighting functionality (single matches, multiple matches)
    - All color options (RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, WHITE)
    - File input/output (reading from files, writing to files)
    - Stdin input (via piping)
    - Prefix and postfix options
    - No-color mode (for compatibility when piping)
    - Edge cases (empty files, files with only newlines, repeated patterns)
    - Complex scenarios (long files, overlapping patterns, whitespace matching)
    - Match counting (statistics printed to stderr: `Highlighted N matches.\n`)
  - **Valgrind inputs (15% = ~3.45 pts)**: Memory safety tests (last 4 tests)
    - Must pass both correctness checks AND valgrind memory checks
    - No memory leaks allowed
    - No invalid memory access errors
    - Proper cleanup of allocated resources

- **Comments (1 pts)**: Effective use of comments explaining your thought process and why code is written as it is. Comments should explain the "why" not just the "what".

- **Git Commits (1 pts)**: Regular commits showing your progress (at least 5 commits). Each commit should represent a meaningful milestone in your implementation.

For late submissions, please refer to syllabus or Canvas website.

## Turnin

Go to Canvas, start the quiz, and submit a commit ID as a single line in the submission box.
