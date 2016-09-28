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


void parse_commond(std::list<Command> &command_list)
{
	char line[80], line_copy[80];
	fgets(line, sizeof(line), stdin);
	strncpy(line_copy, line, sizeof(line_copy));
	puts(line);
	
	char *sep = "\t |"; //separators: space, \t, or pipe
	char *token, *brk_t, *last_token;
	last_token = line_copy;
	
	// printf("address of the init str:%p\n", line);
	
	bool command_flag = true; // the first token should be the command name
	
	Command new_command;
	for (token = strtok_r(line_copy, sep, &brk_t);
		 token;
		 token = strtok_r(NULL, sep, &brk_t))
	{
		bool is_pipe = find_pipe(line, last_token - line_copy, 
									token - line_copy);
		if (is_pipe){
			printf("pipe found between %s and %s", last_token, token);
			command_list.push_back(new_command); // add the last command into list
			new_command = Command(token); // create a new command
		}
		else{
			if (command_flag){
				command_flag = false;
				new_command = Command(token); // create a new command
			}
			else{
				Parameter new_parameter = Parameter(token);
				new_command.add_parameter(new_parameter); // add new para into current command
			}
		}
		last_token = token;
	}
	command_list.push_back(new_command);
}


#ifdef DEBUG
using namespace std;
int main()
{
	while(true){
		list<Command> cl;
		parse_commond(cl);
		list<Command>::iterator iter;
		for (iter = cl.begin(); iter!=cl.end(); ++iter){
			iter->show_parameters();
			cout << "the command name:" << iter->name << endl;
		}		
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