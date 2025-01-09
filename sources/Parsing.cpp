#include <iostream>
#include <string>
#include <cstdlib>
#include "../includes/Server.hpp"
#include "../includes/display.hpp"


void	Server::whoParsing(std::vector<std::string> &params, unsigned int myfd) {
	if (params[0][0] == '#')
		params[0].erase(0, 1);
	size_t pos = params[0].find("\r\n");
	if (pos != std::string::npos)
		params[0] = params[0].substr(0, pos);
	if (isChannel(params[0])) {
		Channel &chan = getChannel(params[0]);
		if (chan.getUser(myfd) == NULL) {
			std::cout << "ERROR : User is not on that channel." << std::endl;
			return;
		}
		std::map<int, User*>::iterator it = chan.getUsers().begin();
		std::cout << Display::GREEN << "Users on channel " << chan.getName() << " :" << std::endl;
		for (; it != chan.getUsers().end(); ++it) {
			// si l'utilisateur est un operateur ajouter un @ devant son nickname
			if (chan.isOperator(it->first))
				std::cout << "@" << it->second->getNickname() << std::endl;
			else
				std::cout << it->second->getNickname() << std::endl;
		}
		std::cout << "End of list" << Display::RESET << std::endl;
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::cout << Display::BLUE << "Operators on channel " << chan.getName() << " :" << std::endl;
		for (std::map<int, User*>::iterator it2 = chan.getOpers().begin(); it2 != chan.getOpers().end(); ++it2) {
			std::cout << it2->first << std::endl;
		}
		std::cout << "End of list" << Display::RESET << std::endl;
	}
}

void	Server::parseTopic(Server &server, int clientFd) {
	if (server._arrayParams.params[0][0] == '#')
		server._arrayParams.params[0].erase(0, 1);
	size_t pos = server._arrayParams.params[0].find("\r\n");
	if (server._arrayParams.params.size() == 1 && pos != std::string::npos)
		server._arrayParams.params[0] = server._arrayParams.params[0].substr(0, pos);
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
	std::string	password;
	password = static_cast<std::string>(argv[2]);

	Server	&server = Server::getInstance(); //Call of server instance
	if(password.compare(server.getPassw())){
		std::cerr << "Bad password, try again." << std::endl;
		exit(1);
	}

}

void	parsing(int argc, char **argv){checkParam(argc, argv);}
