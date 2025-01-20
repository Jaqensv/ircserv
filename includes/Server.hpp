#pragma once
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <sys/types.h>
#include <netdb.h>
#include <sstream> // stringstream
#include <algorithm> // std::find
#include <pwd.h> // getuid, getpwuid
#include "Channel.hpp"
#include "User.hpp"
#include "IrcMessage.hpp"

class Channel;
class User;

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

	//Setter
		void				setPort(unsigned short port);
		void				setPassword(std::string port);
		void				setNeedPasswFalse();
		void				setNeedPasswTrue();
		void				setNameServer(std::string name);
		void				setVersion(std::string version);


	//Getter
		unsigned short			getPort();
		std::string				getPassw();
		const std::vector<Channel*>&	getChannels();
		bool					getNeedPassw();
		unsigned short			getBackLogSize();
		Channel					&getChannel(const std::string channelName);
		bool					isChannel(const std::string &channelName);
		User					&getUser(int fd);
		//void					addServerUser(unsigned int fd);
		std::map<int, User*>&	getUsers();
		std::string				getUsername();
		unsigned int			getTargetUserFd(std::string nickname);
		std::string				getNameServer();
		std::string				getVersion();


		//ahans
		bool					isUser(int fd);
	//commmands functions
		bool	modeCmdParsing(std::vector<std::string> &params, unsigned int myfd);
	//Surcharge operator
		Server	&operator=(Server const &other);

	//Member functions
		void	initServer();
		void	initEpoll();
		void	run();
		void	join(int clientFd);
		void	invite(std::string nickname, std::string channel);
		void	parseTopic(Server &server, int clientFd);
		void	whoParsing(std::vector<std::string> &params, unsigned int myfd);
		int		fillServinfo();


	//Ping Pong Functions
		void	handlePing(int clientFd);

	//Identification functions
		bool		identification(int clientFd);
		bool		identPass(int clientFd);
		bool		askNickname(int clientFd);
		bool		askUser(int clientFd);
		bool		verifCap(int clientFd);
		std::string getCurrentDate();




	//void	createChannel(Channel &chan);
		void	createChannel(Server &server, unsigned int fd, std::string channel_name);
		void	createUser(int fd, User &user);
		void	deleteUser(int fd);

		void	broadcastAll(int senderFd, std::string &message);

	private :
	//Variable member
		struct addrinfo		*_servInfo;
		struct addrinfo		_hints;
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
		std::string			_nameServer;
		std::string			_version;

	//Array of : Channel, User and Operator
		std::vector<Channel*>		_arrayChannel;
		std::map<int, User*>		_arrayUser;
		IrcMessage					_arrayParams;

};
