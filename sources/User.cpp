#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include "../includes/User.hpp"
#include "../includes/Channel.hpp"

//Constructor & Destructor
	User::User() {}
	User::User(User const &copy){(void)copy;}
	User::User(unsigned int fd) : _fd(fd), _buffer(""), _myChannel(""){}
	User::~User(){}

//Surcharge operator
	User	&User::operator=(User const &other){
		this->setFd(other._fd);
		this->setNickname(other._nickname);
		this->setUsername(other._username);
		this->setNickname(other._nickname);
		this->setBuffer(other._buffer);
		return *this;
	}

// Getter
	int							User::getFd(){return this->_fd;}
	std::string					User::getNickname(){return this->_nickname;}
	std::string					User::getUsername(){return this->_username;}
	std::string					User::getBuffer(){return this->_buffer;}
	std::string					User::getMyChannel(){return this->_myChannel;}
	std::vector<std::string>	&User::getMyChannels(){return this->_myChannels;}
	int							User::findChannelIndex(std::string myChannel) {
		int index = 0;
		for (std::vector<std::string>::iterator it = _myChannels.begin(); it != _myChannels.end(); ++it) {
			if (*it == myChannel)
				return index;
			index++;
		}
		return index;
	}


//Setter
	void	User::setFd(unsigned int fd){this->_fd = fd;}
	void	User::setNickname(std::string nickname){this->_nickname = nickname;}
	void	User::setUsername(std::string username){this->_username = username;}
	void	User::setBuffer(std::string buffer){
		if (buffer == "")
			this->_buffer = buffer;
		else
			this->_buffer += buffer;
	};
	void	User::setMyChannel(std::string myChannel){this->_myChannel = myChannel;}

void 	privToUser(std::string user, std::string msg, int clientFd, Server &other)
{
	std::string msgDisplay;
	int usrTgt = other.getTargetUserFd(user);
	if (usrTgt == 0) {
		std::cout << "ERROR: user " << user << " not found" << std::endl;
		return ;
	}
	msgDisplay = other.getUser(clientFd).getNickname() + " -> " + user + " : " + msg;
	send(usrTgt, msgDisplay.c_str(), msgDisplay.size(), 0);
	return ;
};

void 	privToChannel(std::string ChannelName, std::string message, Server &other)
{
	std::string msgDisplay;
	Channel &chan = other.getChannel(ChannelName);
	std::map<int, User*> users = chan.getUsers();
	for (std::map<int, User*>::iterator it = users.begin(); it != users.end(); ++it) {
		msgDisplay = other.getUser(it->first).getNickname() + " -> " + ChannelName + " : " + message;
		send(it->first, msgDisplay.c_str(), msgDisplay.size(), 0);
	}
	return ;
};

void	User::PRIVMSG(std::vector<std::string> &params, unsigned int clientFd, Server &other) {
	std::string msg = "";
	if (params.size() < 2) {
		std::cout << "ERROR: not enougth arguments" << std::endl;
		return ;
	} else {
		if (params[1][0] == ':') {
			unsigned int i = 1;
			while (i < params.size()) {
				msg.append(params[i]);
				msg.append(" ");
				i++;
			}
		} else {
			msg = params[1];
		size_t pos = params[0].find("\r\n");
		if (pos != std::string::npos)
			params[0] = params[0].substr(0, pos);
		} if (params[0][0] != '#') {
			privToUser(params[0], msg, clientFd, other);
			return ;
		} else {
			params[0].erase(0, 1);
			privToChannel(params[0], msg, other);
			return ;
		}
	}
	return ;
};


