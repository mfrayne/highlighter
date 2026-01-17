/* 
 * DISCLAIMER:
 * This program was developed as part of a university course project
 * The project requirements and instructions are outlined in the README file of this project
 * This program demonstrates understanding in basic C conecpts, I/O, and CLI argument parsing
 * All implementation, logic, and code structure are my own work.
 *
 * This program takes an input file and text to be highlighted
 * It then procudes an output file matching the input file with all matches to the input text highlighted
 * The number of matches are provided to the user through stderr
 * Further details can be found in README
 *
 * Psuedo code:
 *
 * Main:
 * 	settings struct is created with defualt values
 * 	cli arguements are parsed
 * 		settings are changed based on flags provided
 *		if anything doesn't follow provided format error is printed and program exits
 *		after all arguments are checked files are opened if specified
 *	lines from input file are parsed
 *		match is found in input file
 *		text before match is printed to ouput
 *		color code and matching text is printed to output 
 *		once all matches are found the remaining text is printed to output
 *	dyanmic memory freed
 *	input and output files are closed if nessecary
 *	matches to text in input file are printed to stderr
 *
 * 	
*/ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>


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
        bool no_color;
        char* prefix;
        char* postfix;
        char* text;
} settings_t;

//function prototypes
void print_help();
void print_error(const char* error_message);
const char* get_color_code(const char* color_str);
int process_args(int argc, char* argv[], settings_t* settings);
int parse_line(settings_t* settings, char* line, FILE* output);


int main(int argc, char *argv[]) {
	
	//stores correctly formated arguments from CLI command
	settings_t settings = {stdin,stdout,COLOR_RED,false,"","",NULL};
	//ensures that user inputs CLI command properly
	process_args(argc,argv,&settings);


	//parse lines of input file and output 
	char* line = NULL;
	size_t len = 0;
	int total_matches = 0;
	//loops through each line rather than reading entire file at once to save memory
	while(getline(&line,&len,settings.input_file) != -1){
		total_matches += parse_line(&settings,line,settings.output_file);
		fflush(settings.output_file);
	}

	//clean up and close files once file reading done
	free(line);
	//if statments ensure that stdin and stdout are not closed
	if(settings.input_file != stdin){
		fclose(settings.input_file);
	}
	if(settings.output_file != stdout){
		fclose(settings.output_file);
	}


	//output of highlighted matches
	fprintf(stderr,"Highlighted %d matches.\n",total_matches);
	return 0;
}

//searches for matches on each line
int parse_line(settings_t* settings, char* line, FILE* output){

	int line_length = strlen(line);
	int text_length = strlen(settings->text);
	int match_count = 0;
	char* match = NULL;
	
	//runs until no matches to the positional argument can be found
	while((match = strstr(line,settings->text)) != NULL){
		//adds the characters between the last match the the pointer to the current character
		//then adds prefix before color change
		fprintf(output,"%.*s%s",(int)(match-line),line,settings->prefix);
		//color of text change if --no color not specified in argument
		if(!settings->no_color){
			fprintf(output,"%s",settings->color);	
		}
		//text for math added to output
		fprintf(output,"%s",settings->text);
		if(!settings->no_color){
			//returns to original color if color was changed when match found
			fprintf(output,"%s",COLOR_RESET_COLOR);
		}
		//postfix added after color returned to normal - empty by default
		fprintf(output,"%s",settings->postfix);
		//moves pointer to after found match
		line = match + strlen(settings->text);
		//match count updated to be later printed
		match_count++;
	}
	//text after last match added to output
	fprintf(output,"%s",line);
	
	return match_count; 
}

//sends specific error message to stderr beginging with Error: and exitis with failure status code
void print_error(const char* error_message){
	fprintf(stderr,"Error: %s\n",error_message);
	exit(-1);
}

//stores correct settings based on cli arguments
int process_args(int argc,char* argv[], settings_t* settings){
	int input_arg = 0;
	int output_arg = 0;
	if(argc<=1){
		print_error("no arguments passed");
	}
	//loops through all arguments passed through to check if everything was inputed corectly
	for (int i = 1; i < argc; i++){
		//handles all flags
		if(argv[i][0] == '-'){
			//help checked first because if contained in args then program ends
			//no need to check rest of arguments if true
			if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
				print_help();
			}
			else if (strcmp(argv[i], "--no-color") == 0) {
                                settings->no_color = true;
                        }
			//all following flags require an argument so if flag has not been caught and is the last element
			//in argv then there is a missing argument for that flag
			else if(i==argc-1){
				print_error("missing flag argument");
			}
			else if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0){
				if(input_arg!=0) print_error("multiple input files specified");
				i++;
				input_arg = i;
			}
			else if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0){
				if(output_arg!=0) print_error("multiple output files specified");
				i++;
				output_arg = i;
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
			//positional argument set if not alreay set
			settings->text = argv[i];
		}
		else{
			//only runs if arg is not a flag and text is already set
			print_error("multiple positional arguments");
		}
	}
	
	
	//ensures positional argument exists and prevents empty string for positional argument
	if(settings->text == NULL||strlen(settings->text)==0){
		print_error("missing positional argument");
	}


	//opens input and output files and ensures open files are closed if there are errors
	//files are only opened after other arguments are checked to be correct
	//so files open files can be closed if there are issues opening one of them

	//input_arg only 0 if no file specified so stdin will be used
	//stdin set as input file by default so no file needs to be opened
	if(input_arg!=0){
		settings->input_file = fopen(argv[input_arg],"r");
		//settings->input_file set to stdin by default so only NULL if fopen fails
		if(settings->input_file== NULL){
			//error prints if issue with openeing 
			//no other files would be open at this point so nothing needs to be closed
			print_error("failed to open input file");
		}
	}
	//output_arg only 0 if no output file specified so stdout will be used
        //stdin set as input file by default so no file needs to be opened
	if(output_arg!=0){
		settings->output_file = fopen(argv[output_arg],"w");
		//output_file is stdout by default so only NULL if fopen fails - usually indicates invalid path
		if(settings->output_file == NULL){
			//closes stdin if output_file was not valid 
			//only nesseary to check for output file because input file is opened before output
			if(settings->input_file!=stdin) fclose(settings->input_file);
			print_error("failed to open output file");
		}
	}
	//success code if no errors encountered with arguments
	return 0;
}



//displays help menu to stdout outlined in README
//multiple printf's used for readability
void print_help(){
	printf("usage: ./highlighter [-h] [-i INPUT] [-o OUTPUT] [-c {RED,GREEN,BLUE,YELLOW,MAGENTA,CYAN,WHITE}]");
		printf("[--no-color] [--prefix PREFIX] [--postfix POSTFIX] text\n");//part of previous line when printed
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

//returns correct global variable color code for the correct text string
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
