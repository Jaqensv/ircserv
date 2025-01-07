#include <iostream>
#include <string>
#include <unistd.h>
#include "Server.hpp"

void	Server::sendPing(int clientFd){

	std::string			pingMessage;;
	std::string			token;
	int					timeToken;
	std::ostringstream	oss;
	(void)clientFd;

	oss << time(NULL);
	token = oss.str();

	std::istringstream iss(token);
	timeToken = 0;
	iss >> timeToken;

	pingMessage = "PING : " + token + "\r\n";

	for(std::map<int, User*>::iterator it = _arrayUser.begin(); it != _arrayUser.end(); it++)
		if(it->first == clientFd){
			it->second->setToken(token);
			it->second->setTimeToken(timeToken);
		}

	if(send(clientFd, pingMessage.c_str(), pingMessage.size(), 0) == -1)
		std::cerr << "ERROR PING : ping message can't be send to client " << clientFd << "." << std::endl;

}

void	Server::handlePong(int clientFd, std::string &token){

	Server	&server = Server::getInstance();
	User	&currentUser = server.getUser(clientFd);

	if(currentUser.getToken() == token)
		currentUser.setToken("");
}


void	Server::handlePing(int clientFd) {
	Server	&server = Server::getInstance();

	if(server._arrayParams.params[0].empty())
		std::cerr << "ERROR CMD PING : need one argument." << std::endl;
	else if(server._arrayParams.params[1].empty())
		std::cerr << "ERROR CMD PING : just need one argument." << std::endl;
	else{
		std::string	pongMessage;
		pongMessage = "PONG : " + server._arrayParams.params[0] + "\r\n";
		send(clientFd, pongMessage.c_str(), pongMessage.size(), 0);
	}
}
