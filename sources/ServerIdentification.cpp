#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>
#include "Server.hpp"
#include "Reply.hpp"

std::string Server::getCurrentDate() {

	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);

	char buffer[11];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", now);

	return std::string(buffer);
}


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

	if (wholePassw.find("\r\n") != std::string::npos)
		wholePassw = wholePassw.substr(0, wholePassw.size() - 2);
	if (wholePassw.find("\n") != std::string::npos)
		wholePassw = wholePassw.substr(0, wholePassw.size() - 1);
	if(cmd.empty() == false && cmd.size() > 5)
		cmd = cmd.substr(0, 4);
	else
		return false;

	if(cmd.compare("PASS") == 0){
		wholePassw = wholePassw.substr(5, (wholePassw.size() - 5));
		if(wholePassw.compare(server.getPassw()) == 0)
			return true;
	}
	std::ostringstream	oss;
	oss << clientFd;
	std::string	stringFd = oss.str();
	send(clientFd, ERR_PASSWDMISMATCH(stringFd).c_str(), ERR_PASSWDMISMATCH(stringFd).size(), 0);
	return false;
}

// bool	Server::askNickname(int clientFd){

// 	Server	&server = Server::getInstance();

// 	char	cmdNickname[100] = {0};
// 	ssize_t	bytesRead;

// 	bytesRead = recv(clientFd, cmdNickname, sizeof(cmdNickname), 0);
// 	if(bytesRead == -1){
// 		std::cerr << "ERROR : nickname's recv doesn't work." << std::endl;
// 		return false;
// 	}

// 	std::string	tmp(cmdNickname);
// 	if (tmp.find("\r\n") != std::string::npos)
// 		tmp = tmp.substr(0, tmp.size() - 2);
// 	if (tmp.find("\n") != std::string::npos)
// 		tmp = tmp.substr(0, tmp.size() - 1);
// 	std::string	wholeCmd;
// 	std::string	cmd = tmp;
// 	std::string	nickname;

// 	if(cmd.empty() == false && cmd.size() > 5)
// 		cmd = cmd.substr(0, 4);
// 	if(cmd.compare("NICK") == 0){
// 		nickname = tmp.substr(5, tmp.size() - 5);
// 		server.getUser(clientFd).setNickname(nickname);
// 	} else {
// 		return false;
// 	}
// 	wholeCmd = ":server_pika NICK " + nickname;
// 	wholeCmd.append("\r\n");

// 	// send(clientFd, wholeCmd.c_str(), wholeCmd.size(), 0);
// 	return true;
// }

// bool	Server::askUser(int clientFd){

// 	char cmdUser[100] = {0};
// 	ssize_t bytesRead = recv(clientFd, cmdUser, sizeof(cmdUser), 0);
// 	if (bytesRead == -1) {
// 		std::cerr << "ERROR : USER command recv failed." << std::endl;
// 		return false;
// 	}

// 	std::string cmd(cmdUser);
// 	std::string userInfo(cmdUser);

// 	if (!cmd.empty() && cmd.size() > 5)
// 		cmd = cmd.substr(0, 4);
// 	if (cmd.compare("USER") == 0){
// 		userInfo = userInfo.substr(5, userInfo.size() - 5);
// 		userInfo = ":server_pika USER " + userInfo + "\r\n";
// 		send(clientFd, userInfo.c_str(), userInfo.size(), 0);
// 		return true;
// 	}
// 	else {
// 		std::cerr << "ERROR: Expected USER command." << std::endl;
// 		return false;
// 	}
// }

// bool	Server::verifCap(int clientFd){

// 	char	cmdCap[100] = {0};
// 	ssize_t	bytesRead;
// 	// std::string	welcome;

// 	bytesRead = recv(clientFd, cmdCap, sizeof(cmdCap), 0);
// 	if(bytesRead == -1){
// 		std::cerr << "ERROR : CAP's recv doesn't work." << std::endl;
// 		return false;
// 	}

// 	std::string wholeCmd(cmdCap);
// 	std::string cmd(cmdCap);
// 	std::string capInfo(cmdCap);

// 	if (!cmd.empty() && cmd.size() > 4)
// 		cmd = cmd.substr(0, 3);

// 	if (cmd.compare("CAP") != 0){
// 		std::cerr << "ERROR: Expected CAP command." << std::endl;
// 		return false;
// 	}
// 	return true;
// }

bool	Server::identification(int clientFd){

	Server	&server = Server::getInstance();
	std::string	welcome;

	// if(verifCap(clientFd) == false)
	// 	return false;
	if(identPass(clientFd) == false)
		return false;
	// if(askNickname(clientFd) == false)
	// 	return false;
	// if(askUser(clientFd) == false)
	// 	return false;


	std::ostringstream	oss;
	oss << clientFd;
	std::string	stringFd = oss.str();
	std::string	date = getCurrentDate();
	send(clientFd, RPL_WELCOME(stringFd, server.getUser(clientFd).getNickname()).c_str(), RPL_WELCOME(stringFd, server.getUser(clientFd).getNickname()).size(), 0);
	send(clientFd, RPL_YOURHOST(stringFd, server.getNameServer(), server.getVersion()).c_str(), RPL_YOURHOST(stringFd, server.getNameServer(), server.getVersion()).size(), 0);
	send(clientFd, RPL_CREATED(stringFd, date).c_str(), RPL_CREATED(stringFd, date).size(), 0);
	send(clientFd, RPL_MYINFO(stringFd, server.getNameServer(), server.getVersion(), "io", "kost", "k").c_str(), RPL_MYINFO(stringFd, server.getNameServer(), server.getVersion(), "io", "kost", "k").size(), 0);
	send(clientFd, RPL_ISUPPORT(stringFd, "CHANNELLEN=32 NICKLEN=9 TOPICLEN=150").c_str(), RPL_ISUPPORT(stringFd, "CHANNELLEN=32 NICKLEN=9 TOPICLEN=150").size(), 0);
	send(clientFd, RPL_MOTDSTART(stringFd, server.getNameServer()).c_str(), RPL_MOTDSTART(stringFd, server.getNameServer()).size(), 0);
	send(clientFd, RPL_MOTD(stringFd, "Hello here is the message of the day").c_str(), RPL_MOTD(stringFd, "Hello here is the message of the day").size(), 0);
	send(clientFd, RPL_ENDOFMOTD(stringFd).c_str(), RPL_ENDOFMOTD(stringFd).size(), 0);

	return true;
}
