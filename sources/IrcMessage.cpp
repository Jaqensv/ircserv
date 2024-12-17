#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include "../includes/IrcMessage.hpp"

void	parseArgs(IrcMessage &mess, std::string &input){

	std::istringstream	iss;
	std::string			command;
	std::string			message;
	std::string			tmp;
	bool				isCrlf = false;
	int					j = 0;
	int					countWord = 0;
	std::string			garbage;

	iss.str(input);
	iss >> mess.command;

	while(iss >> garbage)
		countWord++;
	if(countWord < 1){
		std::cerr << "ERROR CMD : command need almost one parameter." << std::endl;
		mess.params.push_back("");
		return;
	}
	iss.clear();
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
			int	end = mess.params[i].size();
			mess.params[i].erase(end, 1);
			isCrlf = true;
			mess.params[i] += mess.crlf;
		}
		else{
			mess.params.push_back(tmp);
		}
		j = i;
	}
	if(isCrlf == false)
			mess.params[j] += mess.crlf;

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
		int	end = mess.params[i].size();
		mess.params[i].erase(end, 1);
		mess.params[i] += mess.crlf;
	}
	else
		parseArgs(mess, input);

	return mess;
}
