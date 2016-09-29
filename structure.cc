#include "structure.h"


void Command::add_parameter(Parameter p)
{   
    parameters.push_back(p);
}


void Command::show_parameters()
{
    std::list<Parameter>::iterator iter;
    for (iter = parameters.begin(); iter != parameters.end(); iter++){
        std::cout << iter->content << std::endl; 
    }
}

void Command::check_type()
{
	if (name == "cd")
		type = cd;
	else if(name =="exit")
		type = exit;
	else if(name == "jobs")
		type = jobs;
	else
		type = non_built_in;
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