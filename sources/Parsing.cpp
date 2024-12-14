#include <iostream>
#include <string>
#include <cstdlib>
#include "../includes/Server.hpp"


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

void	parsing(int argc, char **argv){

	checkParam(argc, argv);
}
