#include <iostream>
#include <string>
#include "../includes/Operator.hpp"
#include "../includes/Server.hpp"

void	Oper::kick(unsigned int	fd, Channel &channel){channel.removeUser(fd);}

void	Oper::invite(unsigned int fd, User &user, Channel &channel){channel.addUser(fd, user);}

void	Oper::topic(Channel &channel){
	std::cout << channel.getName() << " : " << channel.getTopic() << std::endl;
}

void	Oper::topic(Channel &channel, std::string topic){
	channel.setTopic(topic);
	std::cout << channel.getName() << " topic modified : " << channel.getTopic() << std::endl;
}

void	Oper::changePassw(std::string passw){
	Server	&server = Server::getInstance();
	server.setPassw(passw);
}

void	Oper::deletePassw(){
	Server	&server = Server::getInstance();
	server.setPassw("");
	server.setNeedPasswFalse();
}

void	Oper::useTopic(){
	if(this->_canTopic == false){
		std::cerr << this->getNickname() << " doesn't have permission to change topic." << std::endl;
		return ;
	}
}

void	Oper::Mode(std::string *arg) {
	Server &server = Server::getInstance();
	if (!arg) {
		std::cerr << "ERROR: No argument given for MODE command." << std::endl;
		return ;
	}
	if (arg[0] == "-t") {
		for (int i = 0; i < server.getArrayUser().size(); i++) {
			if (server.getArrayUser()[i].getNickname() == arg[1]) {
				server.getArrayUser()[i].set;
			}
				std::cerr << "ERROR: " << this->getNickname() << " is already an operator." << std::endl;
				return ;
		}
	}
	else if (arg[0] == "-k") {
		this->_canTopic = false;
		std::cout << this->getNickname() << " can't change topic anymore." << std::endl;
	}
	else {
		std::cerr << "ERROR: Invalid argument for MODE command." << std::endl;
	}
}
