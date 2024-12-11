#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include "../includes/IrcMessage.hpp"

void	parseArgs(IrcMessage &mess, std::string &input){

	std::istringstream	iss;
	std::string			command;
	std::string			message;
	std::string			tmp;
	bool				isCrlf = false;

	iss.str(input);
	iss >> mess.command;
	for(int i = 0; iss >> tmp; i++){

		if(tmp[0] == ':'){
			tmp.erase(0,1);
			mess.params.push_back("");
			while(iss >> message){
				mess.params[i] += " ";
				mess.params[i] += message;
			}
			mess.params[i].erase(0, 1);
			isCrlf = true;
			mess.params[i] += mess.crlf;
		}
		else{
			isCrlf = false;
			mess.params.push_back(tmp);
		}
	}
	if (isCrlf == false)
		return;
		// mess.params.back() += mess.crlf;
}

IrcMessage	parseIrcMessage(std::string &input){

	std::istringstream	iss;

	IrcMessage	mess;
	mess.isCommand = true;
	std::string	message;
	mess.crlf = "\r\n";
	int	i = 0;

	iss.str(input);
	if(input[0] != '/'){
		mess.isCommand = false;
		mess.params.push_back("");
		while(iss >> message){
			mess.params[i] += " ";
			mess.params[i] += message;
		}
		mess.params[i].erase(0, 1);
		mess.params[i] += mess.crlf;
		std::cout << "test" << std::endl;
	}
	else{
		iss >> mess.command;
		parseArgs(mess, input);
	}

	return mess;
}
