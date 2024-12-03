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
		std::string				getTopic();
		std::string				getName();
		// matt
		std::map<int, User*>&	getUsers();
		//ahans
		Oper*		getOper(unsigned int fd);
		//ahans
		User*		getUser(unsigned int fd);

	//Setter
		void	setTopic(std::string topic);

	//Member function
		void	addUser(User &user);
		void	removeUser(unsigned int fd);
		void	addOperator(Oper &oper);
		//ahans
		bool	isOperator(unsigned int fd);

	private :
	//Member variable
		std::string				_name;
		std::string				_topic;
		std::map<int, User*> 	_users;
		std::map<int, Oper*>	_operators;
};
