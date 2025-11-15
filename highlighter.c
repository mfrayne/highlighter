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


int main(int argc, char *argv[]) {
	
	//stores correctly formated arguments from CLI command
	settings_t settings = {NULL,NULL,COLOR_RED,false,"","",NULL};
	//ensures that user inputs CLI command properly
	process_args(argc,argv,&settings);
	//open files for input and output
	FILE* in;
	FILE* out;
	//input file
	if(settings.input_file == NULL){
		in = stdin;
	}
	else{
		in = fopen(settings.input_file,"r");
	}
	if(in == NULL){
		print_error("failed to open input file");
	}
	
	//output file
	if(settings.output_file == NULL){
		out = stdout;
	}
	else{
		out = fopen(settings.output_file,"w");
	}
	if(out ==NULL){
		if(in!=stdin){
			fclose(in);//print_error terminates program so previously opened files must be closed
		}
		print_error("failed to open output file");
	}
	//parse lines of input file and output 
	char* line = NULL;
	size_t len = 0;
	int total_matches = 0;
	//loops through each line rather than reading entire file at once to save memory
	while(getline(&line,&len,in) != -1){
		total_matches += parse_line(&settings,line,out);
		fflush(out);
	}
	free(line);
	if(in != stdin){
		fclose(in);
	}
	if(out != stdout){
		fclose(out);
	}
	fprintf(stderr,"Highlighted %d matches.\n",total_matches);
	return 0;
}


int parse_line(settings_t* settings, char* line, FILE* output){
	

	int line_length = strlen(line);
	int text_length = strlen(settings->text);
	int match_count = 0;
	char* match = NULL;
	while((match = strstr(line,settings->text)) != NULL){
		fprintf(output,"%.*s%s",(int)(match-line),line,settings->prefix);
		if(!settings->no_color){
			fprintf(output,"%s",settings->color);	
		}
		fprintf(output,"%s",settings->text);
		if(!settings->no_color){
			fprintf(output,"%s",COLOR_RESET_COLOR);
		}
		fprintf(output,"%s",settings->postfix);
		
		line = match + strlen(settings->text);
		match_count++;
	}
	fprintf(output,"%s",line);
	
	return match_count; 
}

void print_error(const char* error_message){
	fprintf(stderr,"Error: %s\n",error_message);
	exit(-1);
}



int process_args(int argc,char* argv[], settings_t* settings){

	if(argc<=1){
		print_error("no arguments passed");
	}
	//loops through all arguments passed through to check if everything was inputed corectly
	for (int i = 1; i < argc; i++){
		//handles all flags
		if(argv[i][0] == '-'){
			if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
				print_help();
			}
			else if (strcmp(argv[i], "--no-color") == 0) {
                                settings->no_color = true;
                        }
			else if(i==argc-1){
				print_error("missing flag argument");
			}
			else if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0){
				i++;
				settings->input_file = argv[i];
			}
			else if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0){
				i++;
				settings->output_file = argv[i];
			}
			else if(strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--color") == 0){
				i++;
				settings->color = get_color_code(argv[i]);
			}
			else if(strcmp(argv[i], "--prefix")==0){
				i++;
				settings->prefix = argv[i];
			}
			else if(strcmp(argv[i], "--postfix") == 0){
				i++;
				settings->postfix = argv[i];
			}
			else{
				print_error("invalid_flag");
			}
		}
		else if(settings->text==NULL){
			settings->text = argv[i];
		}
		else{
			print_error("postitoial arguments");
		}
	}
	
	

	if(settings->text == NULL||strlen(settings->text)==0){
		print_error("missing positional argument");
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
	exit(1);
}


const char* get_color_code(const char* color_str){
	if(strcmp(color_str,"RED")==0){
		return COLOR_RED;
	}
	else if(strcmp(color_str,"GREEN")==0){
		return COLOR_GREEN;
	}
	else if(strcmp(color_str,"BLUE")==0){
                return COLOR_BLUE;
        }
	else if(strcmp(color_str,"YELLOW")==0){
                return COLOR_YELLOW;
        }
	else if(strcmp(color_str,"MAGENTA")==0){
                return COLOR_MAGENTA;
        }
	else if(strcmp(color_str,"CYAN")==0){
                return COLOR_CYAN;
        }
	else if(strcmp(color_str,"WHITE")==0){
                return COLOR_WHITE;
        }
	else{
		print_error("invalid color argument");
	}
}
