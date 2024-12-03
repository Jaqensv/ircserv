#pragma once
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <sstream> // stringstream
#include <algorithm> // std::find
#include "Channel.hpp"
#include "User.hpp"
#include "Operator.hpp"

class Server{

	public :

	//Instance unique, singleton
	static Server &getInstance() {
		static Server instance;
		return instance;
	}

	//Constructor
		Server();
		Server(Server const &copy);

	//Destructor
		~Server();

	//Setter & Getter
		void				setPort(unsigned short port);
		void				setPassw(std::string port);
		void				setNeedPasswFalse();
		void				setNeedPasswTrue();

		unsigned short			getPort();
		std::string				getPassw();
		std::map<int, User*>	getUsers() const;
		bool					getNeedPassw();
		unsigned short			getBackLogSize();
		//ahans
		Channel					&getChannel(const std::string &channelName);
		//ahans
		bool					isChannel(const std::string &channelName);
		//ahans
		User					&getUser(int fd);
		//ahans
		bool					isUser(int fd);

	//Surcharge operator
		Server	&operator=(Server const &other);

	//Member functions
		void	initServer();
		int		socketNonBlocking(int fd);
		void	initEpoll();
		void	run();

		//void	createChannel(Channel &chan);
		//ahans
		void	createChannel(unsigned int fd, std::string channel_name);
		void	createUser(int fd, User &user);
		void	deleteUser(int fd);

		void	broadcast(int senderFd, std::string &message);


	private :
	//Variable member
		unsigned short		_port;
		std::string			_passw;
		int					_serverSocket;
		struct sockaddr_in	_serverAddres;
		int					_epollFd;
		struct epoll_event	_event;
		socklen_t			_addrlen;
		unsigned short		_backLogSize;
		bool				_invitationOnly;
		bool				_needPassw;

	//Array of : Channel, User and Operator
		std::vector<Channel*>	_arrayChannel;
		std::map<int, User*>	_arrayUser;

};
