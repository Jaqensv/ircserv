#pragma once
#include <iostream>
#include <string>
#include "Server.hpp"
#include "User.hpp"

class Channel{

	public :
	//Constructor & Destructor
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
		std::map<int, User*>&	getOpers();
		//ahans
		User*		getOper(unsigned int fd);
		//ahans
		User*		getUser(unsigned int fd);

	//Setter
		//matt
		void	setTopic(unsigned int fd, std::string topic);

	//Member function
		void	addUser(unsigned int fd);
		void	removeUser(unsigned int fd);
		void	addOperator(unsigned int fd);
		//ahans
		void	revokeOperator(unsigned int clientFd, unsigned int userFd);
		//ahans
		bool	isOperator(unsigned int fd);

	private :
	//Member variable
		Channel();
		std::string				_name;
		std::string				_topic;
		bool					_canTopic; // false pour que les OP, true pour tout le monde

		std::map<int, User*> 	_users;
		std::map<int, User*>	_operators;

};
