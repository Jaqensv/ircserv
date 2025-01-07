#include <iostream>
#include <string>
#include <unistd.h>
#include "Server.hpp"

bool	Server::identPass(int clientFd){

	Server	&server = getInstance();

	char		charPassword[30] = {0};
	std::string	sentencePass;

	send(clientFd, "PASS ", 5, 0);
	ssize_t	bytesRead;
	bytesRead = recv(clientFd, charPassword, sizeof(charPassword) - 1, 0);
	if(bytesRead <= -1)
		std::cerr << "ERROR : RECV password doesn't work." << std::endl;
	charPassword[bytesRead] ='\0';

	std::string	password(charPassword);
	password.erase(password.find_last_not_of("\r\n\t ") + 1);

	if(password.compare(server.getPassw()) != 0){
		std::cerr << "Bad password, try again." << std::endl;
		close(clientFd);
		epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
		deleteUser(clientFd);
		return false;
	}
	return true;
}


bool	Server::identification(int clientFd){

	if(identPass(clientFd) == false)
		return false;


	return true;
}
