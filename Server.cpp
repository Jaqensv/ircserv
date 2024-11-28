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
#include "Server.hpp"
#include "Operator.hpp"
#include "Channel.hpp"

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

	User*			Server::getUser(int fd){
		if (this->_arrayUser.find(fd) != _arrayUser.end()) {
			return _arrayUser[fd];
		}
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

	if(epoll_ctl(server._epollFd, EPOLL_CTL_ADD, server._serverSocket, &server._event) == -1){
		std::cerr << "ERROR EPOLL : epoll_ctl_add failed." << std::endl;
		close(server._serverSocket);
		close(server._epollFd);
		exit(1);
	}
}

//Channel
void	Server::createChannel(Channel &chan){
	this->_arrayChannel.push_back(chan);
}

void	Server::deleteChannel(std::string &channelName){

	for (std::vector<Channel>::iterator it = _arrayChannel.begin(); it != _arrayChannel.end(); ) {
		if (it->getName() == channelName) {
			it = _arrayChannel.erase(it); // Remove and iterator go forward
		} else {
			++it; // Only go forward iterator
		}
	}
}

//User
void	Server::createUser(int fd, User &user){
		this->_arrayUser.insert(std::make_pair(fd, &user));
}

void	Server::deleteUser(int fd){
	this->_arrayUser.erase(fd);
}

//Operator
void	Server::createOperator(Oper &op){
	this->_arrayOperator.push_back(op);
}

void	Server::deleteOperator(int fd){
	for (std::vector<Oper>::iterator it = _arrayOperator.begin(); it != _arrayOperator.end(); ) {
		if (it->getFd() == fd) {
			it = _arrayOperator.erase(it); // Remove and iterator go forward
		} else {
			++it; // Only go forward iterator
		}
	}
}

std::vector<Oper>&	Server::getOperators() {
	return this->_arrayOperator;
}

void	Server::run(){

	// Server	&server = Server::getInstance();

	// struct epoll_event	events[this->_backLogSize];

	while(true){

	}
}
