#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <algorithm>
#include <termios.h>
#include <csignal>
#include <netdb.h>
#include "../includes/IrcMessage.hpp"
#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Tester.hpp"
#include "../includes/handleInclude.hpp"


//Constructor
	Server::Server() : _port(0){
		this->_invitationOnly = false;
		this->setNeedPasswTrue();
	}
	Server::Server(Server const &copy){(void)copy;}

//Destructor
	Server::~Server(){}

//Surcharge operator
	Server	&Server::operator=(Server const &other){(void)other; return (*this);}

//Getter & Setter
	void			Server::setPort(unsigned short port){this->_port = port;}
	unsigned short	Server::getPort(){return this->_port;}

	void			Server::setPassword(std::string password){this->_passw = password;}
	std::string		Server::getPassw(){return this->_passw;};

	void			Server::setNeedPasswFalse(){this->_needPassw = false;}
	void			Server::setNeedPasswTrue(){this->_needPassw = true;}
	bool			Server::getNeedPassw(){return this->_needPassw;}

	const std::vector<Channel*>& Server::getChannels(){
		return _arrayChannel;
	}

//ahans
Channel	&Server::getChannel(const std::string channelName){
	std::vector<Channel*>::iterator it = _arrayChannel.begin();
	for (; it != _arrayChannel.end(); ++it) {
		if (channelName == (*it)->getName()) {
			return **it;
		}
	}
	return **it;
}

//ahans
User	&Server::getUser(int fd) {
	for (std::map<int, User*>::iterator it = this->_arrayUser.begin(); it != _arrayUser.end(); it++) {
		if (it->first == fd)
			return *it->second;
	}
	return *_arrayUser.end()->second;
}

//matt
std::map<int, User*>& Server::getUsers() {return _arrayUser;}

std::string		Server::getUsername() {
	struct passwd *un;
	uid_t uid = getuid();
	un = getpwuid(uid);

	if (un)
		return std::string(un->pw_name);
	return NULL;
}

unsigned int	Server::getTargetUserFd(std::string nickname) {
	for (std::map<int, User*>::iterator user_it = _arrayUser.begin(); user_it != _arrayUser.end(); ++user_it) {
		if (user_it->second->getNickname() == nickname) {
			return user_it->first;
		}
	}
	std::cerr << "Nickname not found" << std::endl;
	return 0;
}

//ahans
bool	Server::isUser(int fd) {
	std::map<int, User*>::iterator it = _arrayUser.begin();
	for (; it != _arrayUser.end(); it++) {
		if (it->first == fd)
			return true;
	}
	return false;
}

unsigned short		Server::getBackLogSize(){
	return this->_backLogSize;
}

bool	Server::isChannel(const std::string &channelName) {
	std::vector<Channel*>::iterator it = _arrayChannel.begin();
	for (; it != _arrayChannel.end(); ++it) {
		if (channelName == (*it)->getName())
			return true;
	}
	return false;
}

//Member functions
void	Server::initServer(){

//Call of server instance
	Server	&server = Server::getInstance();


//Socket creation : for creating communication point, like a FD

	server._serverSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if(server._serverSocket == -1){
		close(server._serverSocket);
		std::cerr << "ERROR SOCKET : Socket can't be created." << std::endl;
		exit(1);
	}

//Allow many sockets on same port
	int opt = 1;
	if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cerr << "ERROR : setsockopt (SO_REUSEADDR) failed." << std::endl;
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
void	Server::createChannel(Server &server, unsigned int fd, std::string channel_name){
	if (isChannel(channel_name) == true) {
		std::cerr << "ERROR: Channel already exists" << std::endl;
		return;
	}
	Channel *newChannel = new Channel(channel_name);

	(*newChannel).addUser(server, fd);
	(*newChannel).addOperator(fd);
	this->_arrayChannel.push_back(newChannel);
}

//User
void	Server::createUser(int fd, User &user){
	if (_arrayUser.find(fd) != _arrayUser.end()) {
	std::cerr << "ERROR FD " << fd << ": already exists in _arrayUser." << std::endl;
	return;
	}
	this->_arrayUser.insert(std::make_pair(fd, &user));
}

void	Server::deleteUser(int fd){
	Server	&server = Server::getInstance();

	if (_arrayUser.find(fd) == _arrayUser.end()) {
		std::cerr << "WARNING : Attempted to delete non-existent user: " << fd << std::endl;
		return;
	}

	std::cout << "Client " << fd << " disconnected." << std::endl;
	if(server.isChannel(getUser(fd).getMyChannel())){
		server.getChannel(getUser(fd).getMyChannel()).removeUser(fd);
	}
	User *user = _arrayUser[fd];
	this->_arrayUser.erase(fd);
	delete user;
}

void	Server::broadcastAll(int senderFd, std::string &message){
	for(std::map<int, User*>::iterator it = this->_arrayUser.begin(); it != _arrayUser.end(); it++){
		int clientFd = it->first;
		if(clientFd != senderFd){
			ssize_t bytesSent = send(clientFd, message.c_str(), message.size(), 0);
			if (bytesSent == -1) {
				std::cerr << "ERROR BROADCAST : Failed to send message to client " << clientFd << std::endl;
				continue;
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
		if(eventCount == -1)
			std::cerr << "ERROR EPOLL_WAIT : epoll_wait doesn't work." << std::endl;

		if (events[0].data.fd == server._serverSocket){ //if connection is about main socket, a new client connection is pending
		//accept client connection
			clientFd = accept(server._serverSocket, (struct sockaddr *)&server._serverAddres, &server._addrlen);
			if(clientFd == -1)
				std::cerr << "ERROR ACCEPT : can't connect to socket." << std::endl;


		//add client to epoll
			struct epoll_event	clientEvent;
			clientEvent.data.fd = clientFd;
			clientEvent.events = EPOLLIN;
			if(epoll_ctl(server._epollFd, EPOLL_CTL_ADD, clientFd, &clientEvent) == -1){
				std::cerr << "ERROR EPOLL : epoll_ctl_add failed." << std::endl;
				continue;
			}

		//add client to client array
			User* newUser = new User(clientFd);
			createUser(clientFd, *newUser);

		//check password, nickname and user
			if(identification(clientFd) == true)
				std::cout << "New client connected : " << clientFd << std::endl;
			else{
				std::cerr << "Bad identification of client : " << clientFd << std::endl;
				close(clientFd);
				epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
				deleteUser(clientFd);
			}
		} else {
		//handle client message
			clientFd = events[0].data.fd;
			char	buffer[512];

			memset(buffer, 0, sizeof(buffer));
			ssize_t	bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
			std::string	mss = buffer;
			if (bytesRead < 0){
				std::cerr << "ERROR RECV : message can't be receive." << std::endl;
				close(clientFd);
				epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
				deleteUser(clientFd);
			} else if (bytesRead == 0){
				close(clientFd);
				epoll_ctl(server._epollFd, EPOLL_CTL_DEL, clientFd, NULL);
				deleteUser(clientFd);
			} else if (mss.empty())
				std::cout << "control D" << std::endl;
			else if (mss[mss.size() - 1] != '\n'){
				server.getUser(clientFd).setBuffer(mss);
				send(clientFd, "^D", 2, 0);
			} else {
				std::string	input = server.getUser(clientFd).getBuffer() + mss;
				server._arrayParams = parseIrcMessage(input);
				if(server._arrayParams.isCommand == false){
					std::cout << server._arrayUser[clientFd]->getNickname() << ": " << server._arrayParams.params[0] << std::flush;
					if(server.isChannel(server.getUser(clientFd).getMyChannel())){
						server.getChannel(server.getUser(clientFd).getMyChannel()).broadcastChannel(clientFd, server._arrayParams.params[0]);
					}
				}
				else if (server._arrayParams.command == "/JOIN")
					join(clientFd);
				else if(server._arrayParams.command == "/PING")
					handlePing(clientFd);
				else if (server._arrayParams.command == "/PART") {
					if (server._arrayParams.params[0][0] == '#')
						server._arrayParams.params[0].erase(0, 1);
					size_t pos = server._arrayParams.params[0].find("\r\n");
					if (pos != std::string::npos)
						server._arrayParams.params[0] = server._arrayParams.params[0].substr(0, pos);
					if (isChannel(server._arrayParams.params[0])) {
						getChannel(server._arrayParams.params[0]).part(clientFd);
					}
				}
				else if (server._arrayParams.command == "/KICK") {
					if (server._arrayParams.params[0][0] == '#')
						server._arrayParams.params[0].erase(0, 1);
					if (isChannel(server._arrayParams.params[0]))
						getChannel(server._arrayParams.params[0]).kick(server, clientFd, server._arrayParams.params[1]);
				} else if (server._arrayParams.command == "/INVITE")
					invite(server._arrayParams.params[0], server._arrayParams.params[1]);
				else if (server._arrayParams.command == "/TOPIC") {
					parseTopic(server, clientFd);
					//channelTopicTester(server._arrayParams.params[0]);
				} else if (server._arrayParams.command == "/MODE")
					modeCmdParsing(server._arrayParams.params, clientFd);
				else if (server._arrayParams.command == "/WHO")
					whoParsing(server._arrayParams.params, clientFd);
				else if (server._arrayParams.command == "/PRIVMSG") {
					getUser(clientFd).PRIVMSG(server._arrayParams.params, clientFd, server);
				}
				else if (server._arrayParams.command == "/MODE")
					std::cout << "Enter MODE methode" << std::endl;
				else if(server._arrayParams.command[0] == '/'){
					std::cout << server._arrayParams.command << " is not a valide command." << std::endl;
					continue;
				}
				server.getUser(clientFd).setBuffer("");
			}
		}
	}
}
