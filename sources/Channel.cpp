#include <iostream>
#include <string>
#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include "../includes/User.hpp"

//Constructor & Destructor
	Channel::Channel(){}
	Channel::Channel(Channel const &copy){(void)copy;}
	Channel::Channel(std::string name) : _name(name), _canTopic(true) {}
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

	void	Channel::removeUser(Server &server, std::string nickname){
		_users.erase(server.getUserFd(nickname));
	}

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

	//matt
	void	Channel::kick(Server &server, unsigned int fd, std::string channel_name, std::string nickname) {
		Channel &channel = server.getChannel(channel_name);
		User* user = server.getChannel(channel_name).getUser(fd);

		if (!server.getChannel(channel_name).isOperator(fd))
			std::cout << user->getNickname() << " doesn't have the rights to kick" << std::endl;
		else {
			channel.removeUser(server, nickname);
		}
	//Parameters: <channel> <user> *( "," <user> ) [<comment>]
	}