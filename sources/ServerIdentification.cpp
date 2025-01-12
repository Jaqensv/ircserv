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
		return false;
	}

	std::string	wholePassw(passRecv);
	std::string	cmd(passRecv);
	std::string	passw(passRecv);

	if(cmd.empty() == false && cmd.size() > 5)
		cmd = cmd.substr(0, 4);
	else
		return false;
	if(cmd.compare("PASS") == 0){
		passw = passw.substr(5, bytesRead - 7);
		if(passw.compare(server.getPassw()) == 0)
			return true;
	}
	else{
		std::string error = ":server_pika 464 * :Incorrect password\r\n";
		send(clientFd, error.c_str(), error.size(), 0);
		return false;
	}
	return true;
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
		return false;
	}

	std::string	wholeCmd(cmdNickname);
	std::string	cmd(cmdNickname);
	std::string	nickname(cmdNickname);

	if(cmd.empty() == false && cmd.size() > 5)
		cmd = cmd.substr(0, 4);
	if(cmd.compare("NICK") == 0){
		nickname = nickname.substr(5, bytesRead - 7);
		server.getUser(clientFd).setNickname(nickname);
	}
	else{
		return false;
	}

	wholeCmd = ":server_pika NICK " + nickname;
	wholeCmd.append("\r\n");

	send(clientFd, wholeCmd.c_str(), wholeCmd.size(), 0);
	return true;
}

bool	Server::askUser(int clientFd){

	char cmdUser[100] = {0};
	ssize_t bytesRead = recv(clientFd, cmdUser, sizeof(cmdUser), 0);
	if (bytesRead == -1) {
		std::cerr << "ERROR : USER command recv failed." << std::endl;
		return false;
	}

	std::string wholeCmd(cmdUser);
	std::string cmd(cmdUser);
	std::string userInfo(cmdUser);

	if (!cmd.empty() && cmd.size() > 5)
		cmd = cmd.substr(0, 4);
	if (cmd.compare("USER") == 0){
		userInfo = userInfo.substr(5, bytesRead - 7);
		wholeCmd = ":server_pika USER " + userInfo + "\r\n";
		send(clientFd, wholeCmd.c_str(), wholeCmd.size(), 0);
		return true;
	}
	else {
		std::cerr << "ERROR: Expected USER command." << std::endl;
		return false;
	}
}



bool	Server::verifCap(int clientFd){

	char	cmdCap[100] = {0};
	ssize_t	bytesRead;
	// std::string	welcome;

	bytesRead = recv(clientFd, cmdCap, sizeof(cmdCap), 0);
	if(bytesRead == -1){
		std::cerr << "ERROR : CAP's recv doesn't work." << std::endl;
		return false;
	}

	std::string wholeCmd(cmdCap);
	std::string cmd(cmdCap);
	std::string capInfo(cmdCap);

	if (!cmd.empty() && cmd.size() > 4)
		cmd = cmd.substr(0, 3);

	if (cmd.compare("CAP") != 0){
		std::cerr << "ERROR: Expected CAP command." << std::endl;
		return false;
	}
	return true;
}

bool	Server::identification(int clientFd){

	Server	&server = Server::getInstance();
	std::string	welcome;

	if(verifCap(clientFd) == false)
		return false;
	if(identPass(clientFd) == false)
		return false;
	if(askNickname(clientFd) == false)
		return false;
	// while(askUser(clientFd) == false)
	// 	continue;

	welcome = ":server_pika 001 " + server.getUser(clientFd).getNickname() + " :Welcome to the Pika network\r\n";
	send(clientFd, welcome.c_str(), welcome.size(), 0);
	welcome = ":server_pika 002 " + server.getUser(clientFd).getNickname() + " :Your host is server_pika, running version 1.0\r\n";
	send(clientFd, welcome.c_str(), welcome.size(), 0);
	welcome = ":server_pika 003 " + server.getUser(clientFd).getNickname() + " :This server was created today\r\n";
	send(clientFd, welcome.c_str(), welcome.size(), 0);
	welcome = ":server_pika 004 " + server.getUser(clientFd).getNickname() + " server_pika 1.0 itkol\r\n";
	send(clientFd, welcome.c_str(), welcome.size(), 0);
	welcome = ":server_pika 005 " + server.getUser(clientFd).getNickname() + " :are supported by this server CHANTYPES=# PREFIX=(ov)@ CHANNELLEN=32 NICKLEN=9 TOPICLEN=307 \r\n";
	send(clientFd, welcome.c_str(), welcome.size(), 0);

	return true;
}
