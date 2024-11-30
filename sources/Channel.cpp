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

//Member function
	void	Channel::mode(){}
	void	Channel::addUser(unsigned int fd, User &user){_users.insert(std::make_pair(fd, &user));}
	void	Channel::removeUser(unsigned int fd){_users.erase(fd);}
	void	Channel::addOperator(unsigned int fd, Oper &oper){_operators.insert(std::make_pair(fd, &oper));}