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
		void	setTopic(unsigned int fd, std::string channel_name, std::string topic);

	//Member function
		void	addUser(unsigned int fd);
		void	removeUser(Server &server, std::string nickname);
		void	addOperator(unsigned int fd);
		//ahans
		void	revokeOperator(unsigned int clientFd, unsigned int userFd);
		//ahans
		bool	isOperator(unsigned int fd);
		//matt
		void	kick(Server &server, unsigned int fd, std::string channel_name, std::string nickname);
		//void	kick(unsigned int fd, std::string channel_name, std::string nickname, std::string comment);

	private :
	//Member variable
		Channel();
		std::string				_name;
		std::string				_topic;
		bool					_canTopic; // false pour que les OP, true pour tout le monde

		std::map<int, User*> 	_users;
		std::map<int, User*>	_operators;

};
