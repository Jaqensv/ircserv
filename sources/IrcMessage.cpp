#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include "../includes/IrcMessage.hpp"

IrcMessage	parseIrcMessage(std::string &input){

	(void)input;
	IrcMessage	mess;
	mess.isCommand = true;
	mess.crlf = "\r\n";

	mess.iss.str(input);
	if(input[0] != '/'){
		mess.isCommand = false;
	}



	return mess;
}
