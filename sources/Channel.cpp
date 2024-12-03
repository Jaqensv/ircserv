#include <iostream>
#include <string>
#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include "../includes/User.hpp"
#include "../includes/Operator.hpp"

//Constructor & Destructor
	Channel::Channel(){}
	Channel::Channel(Channel const &copy){(void)copy;}
	Channel::Channel(std::string name) : _name(name){}
	Channel::~Channel(){}

//Surcharge operator
	Channel	&Channel::operator=(Channel const &other){(void)other; return (*this);}

//Getter & Setter
	std::string	Channel::getTopic(){return this->_topic;}
	void		Channel::setTopic(std::string topic){this->_topic = topic;}
	std::string	Channel::getName(){return this->_name;}

	//ahans
	Oper*		Channel::getOper(unsigned int fd) {
		std::map<int, Oper*>::iterator it = _operators.begin();
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
	void	Channel::addUser(User &user){_users.insert(std::make_pair(user.getFd(), &user));}
	void	Channel::removeUser(unsigned int fd){_users.erase(fd);}
	void	Channel::addOperator(Oper &oper){_operators.insert(std::make_pair(oper.getUser().getFd(), &oper));}

	//ahans
	bool	Channel::isOperator(unsigned int fd){
		std::map<int, Oper*>::iterator it = _operators.begin();
		for (; it != _operators.end(); ++it) {
			if (fd == static_cast<unsigned int>(it->first))
				return true;
		}
		return false;
	}