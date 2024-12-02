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

	// User			&Server::getUser(int fd){
	// 	for (std::map<int, User*>::iterator it = _arrayUser.begin(); it != _arrayUser.end(); ++it){
	// 		if (fd == it->first)
	// 			return *it->second;
	// 	}
		
	// }

	std::map<int, User*>	Server::getUsers() const {
		return _arrayUser;
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

//Channel
// void	Server::createChannel(Channel &chan){
// 	this->_arrayChannel.push_back(chan);
// }

void	Server::createChannel(unsigned int fd, std::string channel_name){
	Channel channel(channel_name);
	Oper oper(fd);

	channel.addUser(fd, oper);
	oper.setNickname("test0");
	std::cout << oper.getNickname() << std::endl;
	channel.addOperator(fd, oper);
	oper.setNickname("test1");
	std::cout << channel.getOper(fd)->getNickname() << std::endl;
	this->_arrayChannel.push_back(channel);
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


std::vector<Channel> Server::getChannels() const {
	return _arrayChannel;
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
					// std::stringstream ss(buffer);
					// std::cout << "Message from client " << clientFd << ": " << buffer;
					// std::string command, channel, arg1, arg2, arg3;
					server.createChannel(clientFd, "new");

					// ss >> command >> channel >> arg1 >> arg2 >> arg3;
					// if (command == "KICK") {
					
					// 	KICK / channel user
					// 	MODE / channel option (facultatif)

					// }
					// // else if (message == "INVITE")
					// // else if (message == "TOPIC")
					// else if (command == "MODE")
					// 	mode(channel, arg1, arg2);
					// // else
					// 	std::cerr << "Error: invalid command" << std::endl;
					//broadcast(clientFd, command);
				}
			}
		}
	}
}

// void Server::execCommand() {
// }
