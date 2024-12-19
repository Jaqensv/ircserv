#include <iostream>
#include <string>
#include "Server.hpp"

void	Server::sendPing(int clientFd){

	std::string			pingMessage;;
	std::string			token;
	std::ostringstream	oss;
	(void)clientFd;

	oss << time(NULL);
	token = "ft_irc_" + oss.str();
	pingMessage = "PING : " + token + "\r\n";

	for(std::map<int, User*>::iterator it = _arrayUser.begin(); it != _arrayUser.end(); it++)
		if(it->first == clientFd)
			it->second->setToken(token);

}
