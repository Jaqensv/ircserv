#include <iostream>
#include <string>
#include <unistd.h>
#include "Server.hpp"

void	Server::handlePing(int clientFd) {
	Server	&server = Server::getInstance();

	if(server._arrayParams.params[0].empty())
		std::cerr << "ERROR CMD PING : need one argument." << std::endl;
	else if(server._arrayParams.params[1].empty())
		std::cerr << "ERROR CMD PING : just need one argument." << std::endl;
	else{
		std::string	pongMessage;
		pongMessage = "PONG : " + server._arrayParams.params[0];
		send(clientFd, pongMessage.c_str(), pongMessage.size(), 0);
	}
}
