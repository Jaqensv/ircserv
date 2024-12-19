#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include "../includes/User.hpp"
#include "../includes/Channel.hpp"

//Constructor & Destructor
	User::User(){}
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
	int			User::getFd(){return this->_fd;}
	std::string	User::getNickname(){return this->_nickname;}
	std::string	User::getUsername(){return this->_username;}
	std::string	User::getBuffer(){return this->_buffer;}
	std::string	User::getMyChannel(){return this->_myChannel;}
	std::string	User::getToken(){return this->_token;};


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
	void	User::setToken(std::string token){this->_token = token;};
