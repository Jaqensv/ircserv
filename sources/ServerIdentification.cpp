#include <iostream>
#include <string>
#include <unistd.h>
#include "Server.hpp"

bool	Server::identPass(int clientFd){

	Server	&server = Server::getInstance();

	char	passRecv[100] = {0};
	ssize_t	bytesRead;

	bytesRead = recv(clientFd, passRecv, sizeof(passRecv), 0);
	if(bytesRead == -1){
		std::cerr << "ERROR :password's recv doesn't work." << std::endl;
		close(clientFd);
		epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
		deleteUser(clientFd);
	}

	std::string	wholePassw(passRecv);
	std::string	cmd(passRecv);
	std::string	passw(passRecv);

	if(cmd.empty() == false && cmd.size() > 5)
		cmd = cmd.substr(0, 4);
	else
		return false;

	if(cmd.compare("PASS") == 0){
		passw = passw.substr(5, bytesRead - 6);
		if(passw.compare(server.getPassw()) == 0)
			return true;
	}
	else
		return false;

	wholePassw = ": PASS " + passw;
	wholePassw.append("\r\n");

	send(clientFd, wholePassw.c_str(), wholePassw.size(), 0);

	return false;
}


bool	Server::askNickname(int clientFd){

	// (void)clientFd;
	Server	&server = Server::getInstance();

	char	cmdNickname[100] = {0};
	ssize_t	bytesRead;
	std::string	welcome;

	bytesRead = recv(clientFd, cmdNickname, sizeof(cmdNickname), 0);
	if(bytesRead == -1){
		std::cerr << "ERROR : nickname's recv doesn't work." << std::endl;
		close(clientFd);
		epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
		deleteUser(clientFd);
	}

	std::string	wholeCmd(cmdNickname);
	std::string	cmd(cmdNickname);
	std::string	nickname(cmdNickname);

	if(cmd.empty() == false && cmd.size() > 5)
		cmd = cmd.substr(0, 4);
	if(cmd.compare("NICK") == 0){
		nickname = nickname.substr(5, bytesRead - 6);
		server.getUser(clientFd).setNickname(nickname);
	}
	else
		return false;

	wholeCmd = ": oldnick nickname newnick";
	wholeCmd.append("\r\n");

	send(clientFd, wholeCmd.c_str(), wholeCmd.size(), 0);
	return true;
}

bool	Server::identification(int clientFd){

	Server	&server = Server::getInstance();
	std::string	welcome;

	while (askNickname(clientFd) == false)
		continue;

	welcome = ":server_pika 001 " + server.getUser(clientFd).getNickname() + " :Welcome to the Pika network\r\n";
	send(clientFd, welcome.c_str(), welcome.size(), 0);
	welcome = ":server_pika 002 " + server.getUser(clientFd).getNickname() + " :Your host is server_pika, running version 1.0\r\n";
	send(clientFd, welcome.c_str(), welcome.size(), 0);
	welcome = ":server_pika 003 " + server.getUser(clientFd).getNickname() + " :This server was created today\r\n";
	send(clientFd, welcome.c_str(), welcome.size(), 0);
	welcome = ":server_pika 004 " + server.getUser(clientFd).getNickname() + " server_pika 1.0 itkol\r\n";
	send(clientFd, welcome.c_str(), welcome.size(), 0);

	while (identPass(clientFd) == false)
		continue;

	return true;
}
