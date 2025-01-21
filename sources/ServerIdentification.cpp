#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>
#include <cstring>
#include "Server.hpp"
#include "User.hpp"
#include "Reply.hpp"

std::string Server::getCurrentDate() {

	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);

	char buffer[11];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", now);

	return std::string(buffer);
}


bool	Server::identPass(int clientFd, std::string passRecv){

	Server	&server = Server::getInstance();

	std::string	wholePassw = passRecv;
	std::string	cmd = passRecv;

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
		if(wholePassw.compare(server.getPassw()) == 0){
			server.getUser(clientFd).setRegisterPassTrue();
			return true;
		}
	}
	std::ostringstream	oss;
	oss << clientFd;
	std::string	stringFd = oss.str();
	send(clientFd, ERR_PASSWDMISMATCH(stringFd).c_str(), ERR_PASSWDMISMATCH(stringFd).size(), 0);
	return false;
}

bool	Server::askNickname(int clientFd, std::string input){

	Server	&server = Server::getInstance();

	std::string	tmp = input;
	if (tmp.find("\r\n") != std::string::npos)
		tmp = tmp.substr(0, tmp.size() - 2);
	if (tmp.find("\n") != std::string::npos)
		tmp = tmp.substr(0, tmp.size() - 1);
	std::string	wholeCmd;
	std::string	cmd = tmp;
	std::string	nickname;

	if(cmd.empty() == false && cmd.size() > 5)
		cmd = cmd.substr(0, 4);
	if(cmd.compare("NICK") == 0){
		nickname = tmp.substr(5, tmp.size() - 5);
		server.getUser(clientFd).setNickname(nickname);
	} else {
		return false;
	}
	wholeCmd = ":server_pika NICK " + nickname;
	wholeCmd.append("\r\n");

	send(clientFd, wholeCmd.c_str(), wholeCmd.size(), 0);
	server.getUser(clientFd).setRegisterNickTrue();
	return true;
}

bool	Server::askUser(int clientFd, std::string input){

	Server	&server = Server::getInstance();

	std::string cmd = input;
	std::string userInfo = input;

	if (!cmd.empty() && cmd.size() > 5)
		cmd = cmd.substr(0, 4);
	if (cmd.compare("USER") == 0){
		userInfo = userInfo.substr(5, userInfo.size() - 5);
		userInfo = ":server_pika USER " + userInfo + "\r\n";
		send(clientFd, userInfo.c_str(), userInfo.size(), 0);
		server.getUser(clientFd).setRegisterUserTrue();
		return true;
	}
	else {
		std::cerr << "ERROR: Expected USER command." << std::endl;
		return false;
	}
}

bool	Server::verifCap(int clientFd, std::string input){

	Server	&server = Server::getInstance();

	std::string wholeCmd = input;
	std::string cmd = input;
	std::string capInfo = input;

	if (!cmd.empty() && cmd.size() > 4)
		cmd = cmd.substr(0, 3);

	if (cmd.compare("CAP") != 0){
		std::cerr << "ERROR: Expected CAP command." << std::endl;
		return false;
	}
	server.getUser(clientFd).setRegisterCapTrue();
	return true;
}


void	Server::sendCap(int clientFd){

	Server	&server = Server::getInstance();

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
}


bool	Server::identification(int clientFd, std::string input){

	Server	&server = Server::getInstance();

	//handle NC
	if(input.find("\r\n") == std::string::npos){
		if(server.getUser(clientFd).getRegisterCap() != true){
			if(verifCap(clientFd, input) == false){
				close(clientFd);
				epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
				deleteUser(clientFd);
			}
		}
		else if(server.getUser(clientFd).getRegisterPass() != true && server.getUser(clientFd).getRegisterCap() == true){
			if(identPass(clientFd, input) == false){
				close(clientFd);
				epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
				deleteUser(clientFd);
			}
		}
		else if(server.getUser(clientFd).getRegisterNick() != true && server.getUser(clientFd).getRegisterPass() == true){
			if(askNickname(clientFd, input) == false){
				close(clientFd);
				epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
				deleteUser(clientFd);
			}
		}
		else if(server.getUser(clientFd).getRegisterUSer() != true && server.getUser(clientFd).getRegisterNick() == true){
			if(askUser(clientFd, input) == false){
				close(clientFd);
				epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
				deleteUser(clientFd);
			}
			sendCap(clientFd);
		}
	}//handle IRSSI
	else{
		if(server.getUser(clientFd).getRegisterCap() != true){
			if(verifCap(clientFd, input) == false){
				close(clientFd);
				epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
				deleteUser(clientFd);
			}
		}
		else if(server.getUser(clientFd).getRegisterPass() != true && server.getUser(clientFd).getRegisterCap() == true){
			if(identPass(clientFd, input.substr(0, input.find("\r\n"))) == false){
				close(clientFd);
				epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
				deleteUser(clientFd);
				return false;
			}
			std::string	cut = input.substr(input.find("\r\n") + 2, input.size() - input.find("\r\n") + 2);
			if(askNickname(clientFd, cut.substr(0, cut.find("\r\n"))) == false){
				close(clientFd);
				epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
				deleteUser(clientFd);
				return false;
			}
			cut = cut.substr(cut.find("\r\n") + 2, cut.size() - cut.find("\r\n") + 2);
			if(askUser(clientFd, cut.substr(0, cut.find("\r\n"))) == false){
				close(clientFd);
				epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
				deleteUser(clientFd);
				return false;
			}
			sendCap(clientFd);
		}

	}

	return true;
}
