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

	// if(password.size() < 6){
	// 	std::cerr << "Password must have at least 6 characters, including at least 1 number and 1 uppercase letter." << std::endl;
	// 	exit(1);
	// }

	// bool	oneDigit = false;
	// bool	oneUpper = false;
	// for(i = 0; i < password.size(); i++){
	// 	if(isdigit(password[i]))
	// 		oneDigit = true;
	// 	else if(isupper(password[i]))
	// 		oneUpper = true;
	// 	if (oneDigit && oneUpper)
	// 		break;
	// }

	// if(!oneDigit || !oneUpper){
	// 	std::cerr << "Password must have at least 6 characters, including at least 1 number and 1 uppercase letter." << std::endl;
	// 	exit(1);
	// }
	Server	&server = Server::getInstance(); //Call of server instance
	if(password.compare(server.getPassw())){
		std::cerr << "Bad password, try again." << std::endl;
		exit(1);
	}
}

void	parsing(int argc, char **argv){

	checkParam(argc, argv);
}
