#include "structure.h"


void Command::add_parameter(Parameter p)
{   
    parameters.push_back(p);
}


void Command::show_parameters()
{
    for (auto iter = parameters.begin(); iter != parameters.end(); iter++){
        std::cout << iter->content << std::endl; 
    }
}

CommandType Command::check_type()
{
	if (name == "cd")
		type = CD;
	else if(name =="exit")
		type = EXIT;
	else if(name == "jobs")
		type = JOBS;
	else if(name == "fg")
		type = FG;
	else if(name == "bg")
		type = BG;
	else if(name == "")
		type = EMPTY;
	else
		type = NON_BUILT_IN;
	return type;
}

size_t Command::parameter_len()
{
	return parameters.size();
}



// #ifdef DEBUG
// using namespace std;
// int main(void)
// {    
//     list<Command> l;
//     Command c = Command("test");
//     c.add_parameter(Parameter("spam"));
//     c.add_parameter(Parameter("fooooooo"));
//     c.show_parameters();
//     return 0;
// }
// #endif