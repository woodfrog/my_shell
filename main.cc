#include <stdio.h>
#include "command_parser.h"
#include "prompt.h"
#include "common.h"
#include <list>

int main(void)
{
	while(true)
	{
		print_prompt();
		
		std::list<Command> commond_list;
		
		parse_commond(commond_list);


		
		// if (fork() != 0){
		// // parent
		
		// }		
		// else{
		// // child
		
		// }

	}	
	return 0;
}
