#include <iostream>
#include <string>
#include <cstdlib>
#include "../includes/Server.hpp"


void	Server::parseTopic(Server &server, int clientFd) {
	if (server._arrayParams.params[0][0] == '#')
		server._arrayParams.params[0].erase(0, 1);
	if (server._arrayParams.params.size() == 1 && server._arrayParams.params[0].find("\r\n"))
		server._arrayParams.params[0] = server._arrayParams.params[0].substr(0, server._arrayParams.params[0].size() - 2);
	if (isChannel(server._arrayParams.params[0])) {
		if (server._arrayParams.params.size() >= 1)
			getChannel(server._arrayParams.params[0]).setTopic(clientFd, server._arrayParams.params);
		else
			getChannel(server._arrayParams.params[0]).setTopic(clientFd);
	}
}

static void	checkParam(int argc, char **argv){

//Number argc verification
	if(argc != 3){
		std::cerr << "ircserv need 2 arguments : <port> <password>" << std::endl;
		exit (1);
	}

// Port verification
	unsigned short	i = 0;
	unsigned int	port;
	while(argv[1][i]){
		if(i > 4){
			std::cerr << "Port must be between 0 and 65535." << std::endl;
			exit (1);
		}
		if(isdigit(argv[1][i]) == 0){
			std::cerr << "Port must be made up of numbers only." << std::endl;
			exit (1);
		}
		i++;
	}

	port = static_cast<unsigned short>(atoi(argv[1]));
	if(port < 1024 || port > 49151){
		std::cerr << "Port must be between 1024 and 49151 for user applications.\
		\nPorts between 0 and 1023 ports are reserved for system services.\
		\nPorts between 49152 and 65535 are reserved for dynamic connections." << std::endl;
		exit(1);
	}
	else{
		Server &server = Server::getInstance();
		server.setPort(port);
	}

//Password verification
	std::string	password(argv[2]);

	Server &server = Server::getInstance();
	server.setPassword(password);
}

void	parsing(int argc, char **argv){checkParam(argc, argv);}
