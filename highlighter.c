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

//settings for highlighting determined by cli command options
typedef struct Settings{
        FILE* input_file;
        FILE* output_file;
        const char* color;
        int no_color;
        char* prefix;
        char* postfix;
        char* text;
} settings_t;

// These function prototypes / definitions are suggestions but not required to implement.
// typedef struct Settings with fields FILE*/handle input_stream, output_stream; strings search_text, prefix, postfix; and boolean no_color;
void print_help();
void print_error(const char* error_message);
void output_final_result(int count, const char* search_text);
const char* get_color_code(const char* color_str);
int process_args(int argc, char* argv[], settings_t* settings);
int parse_line(settings_t* settings, char* line);
//int full_write(int fd,void* data,size_t data_size);
//int full_read(int fd,void* data,size_t data_size)
// ...


int main(int argc, char *argv[]) {
	//parse arguements - get everything from cli arguments and make sure everythings checks out
	//read input from file specified
	//find matches in file
	//add codes to highlight with specified color 
	//write output to stdout or specified file
	//write statistics to stdERR
	
	settings_t settings = {stdin,stdout,COLOR_RED,0,"","","",};

	process_args(argc,argv,&settings);
	
	return 0;
}

void print_error(const char* error_message){
	printf("Error: %s",error_message);
}


//helper function for process_args
//used to valididate the the file path given is a valid file
bool is_valid_file(char* path){
	//check file path for valid file
}
int process_args(int argc,char* argv[], settings_t* settings){
	for (int i = 1; i < argc; i++) {
    		if (strncmp(argv[i], "-h\0",3) == 0 || strncmp(argv[i], "--help",6) == 0) {
                        print_help();
                        return 0;
                }
		else if(i == argc-1){
			settings->text = argv[i];
		}
		//handles options 
		else{
			if (strncmp(argv[i], "-i\0",3) == 0 || strncmp(argv[i], "--input",7) == 0) {
                       		i++;
				// check following file is valid
				if(is_valid_file(argv[i])){
					//handle correct input
				}
				else{
					//handle error of wrong file path and input error message 
				}
                	}
			else if (strncmp(argv[i], "-o\0",3) == 0 || strncmp(argv[i], "--output",7) == 0) {
                        	//check folling file is valid
                        	return 0;
                	}
			else if (strncmp(argv[i], "-c\0",3) == 0 || strncmp(argv[i], "--color",7) == 0) {
				//check following color is valid
				//hanlde color 
                	}
			else if (strncmp(argv[i], "--no-color",7) == 0) {
				//handle no color
    			}
			else if (strncmp(argv[i], "--prefix",7) == 0) {
				//handle prefix
			}
			else if (strncmp(argv[i], "--help",7) == 0) {
				//handle postfix
			}
			else{
				print_error("invalid argument");
				exit(-1);
			}
		}
	}
	return 0;
}


void print_help(){
	printf("usage: ./highlighter [-h] [-i INPUT] [-o OUTPUT] [-c {RED,GREEN,BLUE,YELLOW,MAGENTA,CYAN,WHITE}]");
		printf("[--no-color] [--prefix PREFIX] [--postfix POSTFIX] text\n");
	printf("\n");
	printf("highlighter\n");
	printf("\n");
	printf("positional arguments:\n");
	printf("  text			text to match and highlight\n");
	printf("\n");
	printf("options:\n");
	printf("  -h, --help		show this help message and exit\n");
	printf("  -i INPUT, --input INPUT\n");
	printf("			input file (default:stdin)\n");
	printf("  -o OUTPUT, --output OUTPUT\n");
	printf("			output file (default:stdout)\n");
	printf("  -c {RED,GREEN,BLUE,YELLOW,MAGENTA,CYAN,WHITE}, --color {RED,GREEN,BLUE,YELLOW,MAGENTA,CYAN,WHITE}\n");
        printf("			color option (default: RED)\n");
	printf("  --no-color		do not print color\n");
	printf("  --prefix PREFIX	print prefix before highlighted text (default: empty string\n");
	printf("  --postfix POSTFIX	print postfix after highlighted text (default: tempty string\n");
	printf("\n");
	printf("examples:\n");
	printf("  ./highlighter -i file.txt \"error\" (highlishts \"error\" in file.txt in red via STDOUT\n");
	printf("  ./highlighter\n");
	printf("  ./highlighter --prefix \">>\" --postfix \"<<\" \"keyword\" -o out.txt (highlights \"keyword\" with"); 
		printf(" prefix \">>\" and postfix \"<<\" in red from STDIN to out.txt)\n");
	exit(0);
}



