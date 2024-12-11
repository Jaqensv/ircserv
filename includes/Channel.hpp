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
		void	removeUser(unsigned int fd);
		void	addOperator(unsigned int fd);
		//ahans
		void	revokeOperator(unsigned int clientFd, unsigned int userFd);
		//ahans
		bool	isOperator(unsigned int fd);
		//ahans
		void	switchCanTopic(bool val);
		//ahans
		void	switchInvOnly(bool val);
		//ahans
		void	switchKeyMode();
		//ahans
		void	switchKeyMode(std::string key);
		//ahans
		void	switchLimitMode();
		//ahans
		void	switchLimitMode(int limit);

	private :
	//Member variable
		Channel();
		std::string				_name;
		std::string				_topic;
		bool					_canTopic; // false pour que les OP, true pour tout le monde
		bool					_invOnly;
		bool					_keyMode;
		bool					_limitMode;
		int						_limit;
		std::string				_key;
		std::map<int, User*> 	_users;
		std::map<int, User*>	_operators;

};
