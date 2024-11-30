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
