#include <stdio.h>
#include <stdbool.h>
#include "commond_parser.h"
#include "prompt.h"

int main(void)
{
	while(true)
	{
		print_prompt();
		parse_commond(commond, parameters);
		
		
		if (fork() != 0){
		// parent
		
		}		
		else{
		// child
		
		}

	}	
	return 0;
}
