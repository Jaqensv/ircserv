#pragma once
#include <iostream>
#include <string>
#include "Server.hpp"
#include "User.hpp"

class Server;

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
		std::map<int, User*>& 	getInvited();
		//ahans
		User*		getOper(unsigned int fd);
		//ahans
		User*		getUser(unsigned int fd);
		bool		isInvited(int fd);
		bool		isKeyMode();
		std::string	getKey();
		bool		isLimitMode();
		bool		isInvOnly();
		size_t		getLimit();
		bool		getIsTopic();

	//Setter
		//matt
		void	setTopic(unsigned int fd, std::vector<std::string> topic);
		void	setTopic(unsigned int fd);

	//Member function
		void	addUser(Server &server, unsigned int fd);
		void	removeUser(int clientFd);
		void	addOperator(unsigned int fd);
		void	kick(Server &server, unsigned int fd, std::string nickname);

		//ahans
		void	revokeOperator(unsigned int clientFd, unsigned int userFd);
		bool	isOperator(unsigned int fd);
		void	switchCanTopic(bool val);
		void	switchInvOnly(bool val, int fd);
		void	switchKeyMode();
		void	switchKeyMode(std::string key);
		void	switchLimitMode();
		void	switchLimitMode(int limit);
		void	broadcastChannel(int senderFd, std::string &message);


	private :
	//Member variable
		Channel();
		std::string				_name;
		std::string				_topic;
		bool					_canTopic; // false pour que les OP, true pour tout le monde
		bool					_invOnly;
		bool					_keyMode;
		bool					_limitMode;
		size_t					_limit;
		std::string				_key;
		std::map<int, User*> 	_users;
		std::map<int, User*>	_operators;
		std::map<int, User*>	_invited;

};
