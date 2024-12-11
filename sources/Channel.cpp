#include <iostream>
#include <string>
#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include "../includes/User.hpp"

//Constructor & Destructor
	Channel::Channel(){}
	Channel::Channel(Channel const &copy){(void)copy;}
	Channel::Channel(std::string name) : _name(name), _canTopic(true), _invOnly(false), _keyMode(false), _limitMode(false) {}
	Channel::~Channel(){}

//Surcharge operator
	Channel	&Channel::operator=(Channel const &other){(void)other; return (*this);}

//Getter & Setter
	std::string				Channel::getTopic(){return this->_topic;}
	std::string				Channel::getName(){return this->_name;}
	std::map<int, User*>&	Channel::getUsers(){return _users;}
	std::map<int, User*>&	Channel::getOpers(){return _operators;}
	//matt
	void					Channel::setTopic(unsigned int fd, std::string channel_name, std::string topic) {
		Server &server = server.getInstance();

		std::map<int, User*>::iterator user_it = server.getChannel(channel_name)._users.find(fd);
		if (server.getChannel(channel_name)._canTopic == true) {
			server.getChannel(channel_name)._topic = topic;
			std::cout << user_it->second->getUsername() << " has changed the topic: " << _topic << std::endl;
		}
		else {
			if (server.getChannel(channel_name).isOperator(fd)) {
				server.getChannel(channel_name)._topic = topic;
				std::cout << user_it->second->getUsername() << " has changed the topic: " << _topic << std::endl;
			}
			else
				std::cout << user_it->second->getUsername() <<  " doesn't have the rights to change the topic" << std::endl;
		}
	}

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

	//ahans
	void	Channel::switchCanTopic(bool val){
		if (val != _canTopic) {
			if (val == true)
				std::cout << "TOPIC restriction has been added" << std::endl;
			else
				std::cout << "TOPIC restriction has been lift" << std::endl;
			_canTopic = val;
		} else
			std::cout << "TOPIC rights are already at this state" << std::endl;
	}
	//ahans
	void	Channel::switchInvOnly(bool val) {
		if (val != _invOnly) {
			if (val == true)
				std::cout << "Mode invite only is on" << std::endl;
			else
				std::cout << "Mode invite only is off" << std::endl;
			_invOnly = val;
		} else
			std::cout << "Mode invite only is already at this state" << std::endl;
	}
	//ahans
	void	Channel::switchKeyMode() {
		if (_keyMode == false) {
			std::cout << "Mode key is already off" << std::endl;
		} else {
			std::cout << "Mode key is off" << std::endl;
			_keyMode = false;
		}
	}
	//ahans
	void	Channel::switchKeyMode(std::string key) {
		if (_keyMode == false) {
			std::cout << "Mode key is on" << std::endl;
			_keyMode = true;
			_key = key;
		} else {
			std::cout << "You change password" << std::endl;
			_key = key;
		}
	}
	//ahans
	void	Channel::switchLimitMode() {
		if (_limitMode == false) {
			std::cout << "Mode limit is already off" << std::endl;
		} else {
			std::cout << "Mode limit is off" << std::endl;
			_limitMode = false;
		}
	}
	//ahans
	void	Channel::switchLimitMode(int limit) {
		if (_limitMode == false) {
			std::cout << "Mode limit is on" << std::endl;
			_limitMode = true;
			_limit = limit;
		} else {
			std::cout << "You change limit" << std::endl;
			_limit = limit;
		}
	}