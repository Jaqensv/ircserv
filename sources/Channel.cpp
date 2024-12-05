#include <iostream>
#include <string>
#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include "../includes/User.hpp"

//Constructor & Destructor
	Channel::Channel(){}
	Channel::Channel(Channel const &copy){(void)copy;}
	Channel::Channel(std::string name) : _name(name) {}
	Channel::~Channel(){}

//Surcharge operator
	Channel	&Channel::operator=(Channel const &other){(void)other; return (*this);}

//Getter & Setter
	std::string				Channel::getTopic(){return this->_topic;}
	void					Channel::setTopic(std::string topic){this->_topic = topic;}
	std::string				Channel::getName(){return this->_name;}
	std::map<int, User*>&	Channel::getUsers(){return _users;}
	std::map<int, User*>&	Channel::getOpers(){return _operators;}

	//ahans
	User*		Channel::getOper(unsigned int fd) {
		std::map<int, User*>::iterator it = _operators.begin();
		for (; it != _operators.end(); ++it) {
			if (fd == static_cast<unsigned int>(it->first))
				return it->second;
		}
		return NULL;
	}

	//ahans
	User*		Channel::getUser(unsigned int fd) {
		std::map<int, User*>::iterator it = _users.begin();
		for (; it != _users.end(); ++it) {
			if (fd == static_cast<unsigned int>(it->first))
				return it->second;
		}
		return NULL;
	}

//Member function
	void	Channel::addUser(unsigned int fd){
		Server	&server = Server::getInstance();
		_users.insert(std::make_pair(fd, &server.getUser(fd)));
	}

	void	Channel::addOperator(unsigned int fd) {
		_operators.insert(std::make_pair(fd, getUser(fd)));
	}

	void	Channel::removeUser(unsigned int fd){_users.erase(fd);}
	//ahans
	void	Channel::revokeOperator(unsigned int clientFd, unsigned int userFd){
		if (isOperator(clientFd))
			_operators.erase(userFd);
		else
			std::cout << "User " << clientFd << " is not operator can't revoke user " << userFd << std::endl;
	}

	//ahans
	bool	Channel::isOperator(unsigned int fd){
		std::map<int, User*>::iterator it = _operators.begin();
		for (; it != _operators.end(); ++it) {
			if (fd == static_cast<unsigned int>(it->first))
				return true;
		}
		return false;
	}