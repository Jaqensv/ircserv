#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include "../includes/IrcMessage.hpp"

IrcMessage	parseIrcMessage(std::string &input){
	IrcMessage	mess;
	std::istringstream	iss;
	std::string			tmp;
	mess.crlf = "\r\n";

	if (input[0] == '/')
		input.erase(0, 1);
	iss.str(input);
	for (int i = 0; iss >> tmp; i++) {
		if (i == 0) {
			mess.command = tmp;
		} else if (tmp[0] == ':') {
			mess.params.push_back(tmp);
			while (iss >> tmp) {
				mess.params[i] += " ";
			}
			mess.params[i] += mess.crlf;
		} else {
			mess.params.push_back(tmp);
		}
	}
	tmp = mess.params[mess.params.size() - 1] + mess.crlf;
	mess.params[mess.params.size() - 1] = tmp;
	return mess;
}