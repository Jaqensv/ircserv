#pragma once
#include <iostream>
#include <string>
#include "Server.hpp"
#include "User.hpp"
#include "Operator.hpp"

class Channel{

	public :
	//Constructor & Destructor
		Channel();
		Channel(Channel const &copy);
		Channel(std::string name);
		~Channel();

	//Surcharge operator
		Channel	&operator=(Channel const &other);

	//Getter
		std::string	getTopic();
		std::string	getName();
		Oper*		getOper(unsigned int fd);

	//Setter
		void	setTopic(std::string topic);

	//Member function
		void	mode();
		void	addUser(unsigned int fd, User &user);
		void	removeUser(unsigned int fd);
		void	addOperator(unsigned int fd, Oper &oper);

	private :
	//Member variable
		std::string				_name;
		std::string				_topic;
		std::map<int, User*> 	_users;
		std::map<int, Oper*>	_operators;
};
