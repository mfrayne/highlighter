#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>

/*
This is the implementation of the highlighter program.
Please read the documentation in the README before you start working on this file.
*/

// Color codes to use in your program. Print a color first, then your text, then the reset color.
static const char* COLOR_RED = "\033[0;31m";
static const char* COLOR_GREEN = "\033[0;32m";
static const char* COLOR_BLUE = "\033[0;34m";
static const char* COLOR_YELLOW = "\033[1;33m";
static const char* COLOR_MAGENTA = "\033[0;35m";
static const char* COLOR_CYAN = "\033[0;36m";
static const char* COLOR_WHITE = "\033[1;37m";
static const char* COLOR_RESET_COLOR = "\033[0m";


// These function prototypes / definitions are suggestions but not required to implement.
// typedef struct Settings with fields FILE*/handle input_stream, output_stream; strings search_text, prefix, postfix; and boolean no_color;
// void print_help()
// void print_error(const char* error_message)
// void output_final_result(int count, const char* search_text)
// const char* get_color_code(const char* color_str)
// int process_args(int argc, char* argv[], Settings* settings)
// int parse_line(Settings* settings, char* line)
// ...


int main(int argc, char *argv[]) {
    // TODO: implement the highlighter program
    return 0;
}
