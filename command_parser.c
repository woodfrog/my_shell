#include "command_parser.h"

bool find_pipe(char *s, int start, int end)
{
	if (start == end)
		return false;
	else{
		char *p;
		for (p = s+start; p != s+end; p++){
			if (*p == '|'){
				return true;
			}
		}	
		return false;
	}
}

void parse_commond(Command *commond)
{
	char line[80], line_copy[80];
	fgets(line, sizeof(line), stdin);
	strncpy(line_copy, line, sizeof(line_copy));
	puts(line);
	
	char *sep = "\t |"; //separators: space, \t, or pipe
	char *token, *brk_t, *last_token;
	last_token = line_copy;
	
	printf("address of the init str:%p\n", line);
	
	for (token = strtok_r(line_copy, sep, &brk_t);
		 token;
		 token = strtok_r(NULL, sep, &brk_t))
	{
		bool is_pipe = find_pipe(line, last_token - line_copy, 
									token - line_copy);
		if (is_pipe){
			printf("pipe found between %s and %s", last_token, token);
		}
		else{
			printf("address is:%p, content:%s\n", token, token);
			// operations to deal with the same command 
			// 1. store the command name
			// 2. store the parameters into the list for parameter
		}
		last_token = token;
	}
}


#ifdef DEBUG
int main()
{
	while(true){
		Command *p;
		parse_commond(p);
	}
}
#endif


// int main()
// {
// 	char test[80], blah[80];
// 	char *sep = "\\/:;=-";
// 	char *word, *phrase, *brkt, *brkb;
// 	printf("address of the init str:%p\n", test);
		 
// 	strcpy(test, "This;is.a:test:of=the/string\\tokenizer-function.");
		 
// 	for (word = strtok_r(test, sep, &brkt);
// 		word;
// 		 word = strtok_r(NULL, sep, &brkt))
// 	{
// 		printf("address is:%p, content:%s\n", word, word);
// 	}

// 	return 0;
// }