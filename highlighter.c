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
        char* input_file;
        char* output_file;
        const char* color;
        bool no_color;
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
int parse_line(settings_t* settings, char* line, FILE* output);
// ...

FILE* open_file(char* path, FILE* file){
	FILE* return_file = NULL;
	if(path == NULL){
                return_file = file;
        }
        else{
		return_file = fopen(path,"r");
        }
	return return_file;
}

int main(int argc, char *argv[]) {
	//parse arguements - get everything from cli arguments and make sure everythings checks out
	//read input from file specified
	//find matches in file
	//add codes to highlight with specified color 
	//write output to stdout or specified file
	//write statistics to stdERR
	
	//stores correctly formated arguments from CLI command
	settings_t settings = {NULL,NULL,COLOR_RED,false,"","","",};
	//ensures that user inputs CLI command properly
	process_args(argc,argv,&settings);
	

	//open files for input and output
	FILE* in;
	FILE* out;
	
	//input file
	in = open_file(settings.input_file,stdin);
	if(in == NULL){
		print_error(strncat("failed to open file at ",settings.input_file,256));
	}
	
	//output file
	out = open_file(settings.output_file,stdout);
	if(out ==NULL){
		fclose(out);//print_error terminates program so previously opened files must be closed
		print_error(strncat("failed to open file at ",settings.output_file,256));
	}


	//parse lines of input file and output 
	char* line = NULL;
	size_t len = 0;
	int total_matches = 0;
	//loops through each line rather than reading entire file at once to save memory
	while(getline(&line,&len,in) != -1){
		total_matches += parse_line(&settings,line,out);

	}
	//ensure all memory is handled properly
	free(line);
	if(in != stdin){
		fclose(in);
	}
	if(out != stdout){
		fclose(out);
	}
	//test end of stdout
	fprintf(stdout,"\nend of stdout\n");
	//prints out statistics
	fprintf(stderr,"Highlighted %d matches.\n",total_matches);
	return 0;
}


int parse_line(settings_t* settings, char* line, FILE* output){
	//search line character by character for matches
	//use for loop and strcmp
	//add each character to a new string
	//when match reached
		//add prefix to output
		//add color code if no color = 0
		//add work
		//add reset color code
		//add postfix
		//increase index by word length
	//
	int line_length = strlen(line);
	int text_length = strlen(settings->text);
	int match_count = 0;
	char* match = NULL;
	while((match = strstr(line,settings->text)) != NULL){
		fprintf(output,"%.*s%s",(int)(match-line),line,settings->prefix);
		if(!settings->no_color){
			fprintf(output,"%s",settings->color);	
		}
		fprintf(output,"%s%s%s",settings->text,COLOR_RESET_COLOR,settings->postfix);
		
		
		line = match + strlen(settings->text);
		match_count++;
	}
	fprintf(output,line);
	
	return match_count;
}

void print_error(const char* error_message){
	fprintf(stderr,"Error: %s\n",error_message);
	exit(-1);
}


//helper function for process_args
//used to validiate the the file path given is a valid file
bool is_valid_file(char* path){
	//check file path for valid file
	FILE* file = fopen(path,"r");
	if(file == NULL){
		return false;
	}
	else{
		fclose(file);
		return true;
	}
}
int process_args(int argc,char* argv[], settings_t* settings){
	char* in_path = NULL;
	char* out_path = NULL;

	for (int i = 1; i < argc; i++){
		//prints help menu
		//seperate from other options because positional argument not needed
    		if (strncmp(argv[i], "-h\0",3) == 0 || strncmp(argv[i], "--help",6) == 0) {
                        print_help();
                        return 0;
                }
		//checks for positional argument - text to be highlighted
		else if(i == argc-1){
			settings->text = argv[i];
		}
		//handles option flags 
		else{
			//first becaue it is the only flag besides help that doesn't need an argument
			//handles no color flag
			if (strncmp(argv[i], "--no-color",7) == 0) {
                                settings->no_color = true;
                        }
			else if(i>=argc-2){//all following cases should have an argument following the flag
				//all these flag should not be last or second to last argument
				//if they are they have not includes the positional argument text
				print_error("missing positional argument");
			}
			//need to validate file
			else if (strncmp(argv[i], "-i\0",3) == 0 || strncmp(argv[i], "--input",7) == 0) {
                       		i++;
				// check following file is valid
				if(argv[i][0]=='-'){
                                        print_error("missing argument for -i");
                                }
				else if(is_valid_file(argv[i])){
					in_path = argv[i];
				}
				else{
					print_error(strncat("issue opening file at ",argv[i],10));
				}
                	}
			//need to validate file
			else if (strncmp(argv[i], "-o\0",3) == 0 || strncmp(argv[i], "--output",7) == 0) {
                        	//check folling file is valid
				if(argv[i][0]=='-'){
					print_error("missing argument for -i");
				}
				else if(is_valid_file(argv[i])){
					out_path = argv[i];
				}
                                else{
					print_error(strncat("issue opening file at ",argv[i],10));
                                }
                	}
			else if (strncmp(argv[i], "-c\0",3) == 0 || strncmp(argv[i], "--color",7) == 0) {
				//check following color is valid
				i++;
				//get_color_code used to validate input text and translate to correct ANSI color code
				settings->color = get_color_code(argv[i]);
                	}
			//handles prefix
			else if (strncmp(argv[i], "--prefix",7) == 0) {
				i++;
				//set limit to how long prefix can b
				settings->prefix = argv[i];
			}
			//handles postfix
			else if (strncmp(argv[i], "--postfix",7) == 0) {
				//handle postfix
				i++;
				settings->postfix = argv[i];
			}
			else{//handles any errors that result in extra arguments
				print_error(strncat("multiple position arguments/invalid flag ",argv[i],10));
				return -1;
			}
		}
	}
	
	settings->input_file = in_path;
	settings->output_file = out_path;

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


const char* get_color_code(const char* color_str){
	if(strncmp(color_str,"RED\0",4)){
		return COLOR_RED;
	}
	else if(strncmp(color_str,"GREEN\0",6)){
		return COLOR_GREEN;
	}
	else if(strncmp(color_str,"BLUE\0",6)){
                return COLOR_BLUE;
        }
	else if(strncmp(color_str,"YELLOW\0",6)){
                return COLOR_YELLOW;
        }
	else if(strncmp(color_str,"MAGENTA\0",6)){
                return COLOR_MAGENTA;
        }
	else if(strncmp(color_str,"CYAN\0",6)){
                return COLOR_CYAN;
        }
	else if(strncmp(color_str,"WHITE\0",6)){
                return COLOR_WHITE;
        }
	else{
		print_error(strncat("invalid color argument ",color_str,10));
		return 0;
	}
}
