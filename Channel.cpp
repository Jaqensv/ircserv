#include <iostream>
#include <string>
#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Operator.hpp"

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
