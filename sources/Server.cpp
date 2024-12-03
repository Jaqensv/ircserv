#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <algorithm>
#include "../includes/Server.hpp"
#include "../includes/Operator.hpp"
#include "../includes/Channel.hpp"

//Constructor
	Server::Server() : _port(0){
		this->_invitationOnly = false;
		this->setNeedPasswTrue();
		this->_passw = "3MatAllGui";
	}
	Server::Server(Server const &copy){(void)copy;}

//Destructor
	Server::~Server(){}

//Surcharge operator
	Server	&Server::operator=(Server const &other){(void)other; return (*this);}

//Getter & Setter
	void			Server::setPort(unsigned short port){this->_port = port;}
	unsigned short	Server::getPort(){return this->_port;}

	void			Server::setPassw(std::string password){this->_passw = password;}
	std::string		Server::getPassw(){return this->_passw;};

	void			Server::setNeedPasswFalse(){this->_needPassw = false;}
	void			Server::setNeedPasswTrue(){this->_needPassw = true;}
	bool			Server::getNeedPassw(){return this->_needPassw;}

	std::map<int, User*>	Server::getUsers() const {
		return _arrayUser;
	}

	//ahans
	Channel*	Server::getChannel(const std::string &channelName){
		std::cout << "search channel : " << channelName << std::endl;
		std::vector<Channel*>::iterator it = _arrayChannel.begin();
		for (; it != _arrayChannel.end(); ++it) {
			if (channelName == (*it)->getName()) {
				std::cout << "channel found " << (*it)->getName() << std::endl;
				return *it;
			}
		}
		std::cout << "channel not found" << std::endl;
		return NULL;
	}

	unsigned short		Server::getBackLogSize(){
		return this->_backLogSize;
	}


//Member functions
int	Server::socketNonBlocking(int fd){

	Server	&server = Server::getInstance(); //Call of server instance

//Check if socket creation work
	if(server._serverSocket == -1){
		close(server._serverSocket);
		std::cerr << "ERROR SOCKET : Socket can't be created." << std::endl;
		exit(1);
	}

//Non-blocking mode socket
	int flags = fcntl(fd, F_GETFL, 0); //-1 si bloquant
	if (flags == -1) {
		return -1;
	}
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void	Server::initServer(){

//Call of server instance
	Server	&server = Server::getInstance();


//Socket creation : for creating communication point, like a FD

	server._serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (socketNonBlocking(server._serverSocket) < 0){
	std::cerr << "ERROR: Unable to set server socket to non-blocking mode." << std::endl;
	exit(1);
	}

//Address socket creation with sockaddr_in structure
	memset(&server._serverAddres, 0, sizeof(server._serverAddres));
	server._serverAddres.sin_family = AF_INET; //AF_INET for IPV4
	server._serverAddres.sin_addr.s_addr = INADDR_ANY; //Address to accept any incoming messages
	server._serverAddres.sin_port = htons(server._port); //Convert local data to network data(network communication : big-endian)

//Bind : bound socket with port
	if(bind(server._serverSocket, (struct sockaddr *)&server._serverAddres,sizeof(server._serverAddres)) < 0){
		close(server._serverSocket);
		std::cerr << "ERROR BIND : can't create bound between socket and port." << std::endl;
		exit(1);
	}

//Listen :server able to connect in
	server._backLogSize = 128; //queue size
	if(listen(server._serverSocket, server._backLogSize) < 0){
		close(server._serverSocket);
		std::cerr << "ERROR LISTEN : Unable to listen on the socket." << std::endl;
		exit (1);
	}
}

void	Server::initEpoll(){

	Server	&server = getInstance();

	server._epollFd = epoll_create1(0);
	if (_epollFd == -1) {
		std::cerr << "ERROR EPOLL: epoll_create1 failed." << std::endl;
		close(server._serverSocket);
		exit(1);
	}

	server._event.data.fd = server._serverSocket;
	server._event.events = EPOLLIN | EPOLLOUT;

	if (epoll_ctl(server._epollFd, EPOLL_CTL_ADD, server._serverSocket, &server._event) == -1){
		std::cerr << "ERROR EPOLL : epoll_ctl_add failed." << std::endl;
		close(server._serverSocket);
		close(server._epollFd);
		exit(1);
	}
}

//ahans
void	Server::createChannel(unsigned int fd, std::string channel_name){
	Channel *newChannel = new Channel(channel_name);
	Oper oper(fd);

	(*newChannel).addUser(fd, oper);
	(*newChannel).addOperator(fd, oper);
	this->_arrayChannel.push_back(newChannel);
}

//User
void	Server::createUser(int fd, User &user){
		this->_arrayUser.insert(std::make_pair(fd, &user));
}

void	Server::deleteUser(int fd){
	this->_arrayUser.erase(fd);
}

void	Server::broadcast(int senderFd, std::string &message){
	for(std::map<int, User*>::iterator it =this->_arrayUser.begin(); it != _arrayUser.end(); it++){
		int clientFd = it->first;
		if(clientFd != senderFd){
			ssize_t bytesSent = send(clientFd, message.c_str(), message.size(), 0);
			if (bytesSent == -1) {
				std::cerr << "ERROR BROADCAST : Failed to send message to client " << clientFd << std::endl;
				exit(1);
			}
		}
	}
}

void	Server::run(){

	Server	&server = Server::getInstance();
	server.initEpoll();

	struct epoll_event	events[this->_backLogSize];
	int					eventCount, clientFd;

	server._addrlen = sizeof(server._serverAddres);
	while(true){
		eventCount = epoll_wait(server._epollFd, events, this->_backLogSize, -1);
		if(eventCount == -1){
			std::cerr << "ERROR EPOLL_WAIT : epoll_wait doesn't work." << std::endl;
			close(server._serverSocket);
			close(server._epollFd);
			exit(1);
		}

		for(int n = 0; n < eventCount; n++){
			if (events[n].data.fd == server._serverSocket){ //if connection is about main socket, a new client connection is pending
			//accept client connection
				clientFd = accept(server._serverSocket, (struct sockaddr *)&server._serverAddres, &server._addrlen);
				if(clientFd == -1){
					std::cerr << "ERROR ACCEPT : can't connect to socket." << std::endl;
					close(server._serverSocket);
					close(server._epollFd);
					exit(1);
				}
				if (socketNonBlocking(clientFd) < 0){
					std::cerr << "ERROR: Unable to set server socket to non-blocking mode." << std::endl;
					close(server._serverSocket);
					close(server._epollFd);
					exit(1);
				}

			//add client to epoll
				struct epoll_event	clientEvent;
				clientEvent.data.fd = clientFd;
				clientEvent.events = EPOLLIN;
				if(epoll_ctl(server._epollFd, EPOLL_CTL_ADD, clientFd, &clientEvent) == -1){
					std::cerr << "ERROR EPOLL : epoll_ctl_add failed." << std::endl;
					close(server._serverSocket);
					close(server._epollFd);
					exit(1);
				}

			//add client to client array
				User	newUser(clientFd);
				createUser(clientFd, newUser);

				std::cout << "New client connected : " << clientFd << std::endl;
			}
			else{
			//handle client message
				clientFd = events[n].data.fd;
				char	buffer[1024];

				memset(buffer, 0, sizeof(buffer));
				ssize_t	bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
				if(bytesRead <= 0){
					std::cout << "Client disconnected: " << clientFd << std::endl;
					close(clientFd);
					close(server._serverSocket);
					close(server._epollFd);
					epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
					deleteUser(clientFd);
					exit(1);
				} else {
					server.createChannel(clientFd, "new");
					server.createChannel(clientFd, "new1");
					server.createChannel(clientFd, "new2");
					std::cout << "false :" << std::endl;
					std::cout << server.getChannel("test") << std::endl;
					std::cout << "true :" << std::endl;
					std::cout << server.getChannel("new2") << std::endl;
				}
			}
		}
	}
}
