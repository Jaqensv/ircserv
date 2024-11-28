#include <iostream>
#include <string>
#include "../includes/Operator.hpp"
#include "../includes/Server.hpp"


void	Oper::kick(unsigned int fd){
	(void)fd;
}


void	Oper::invite(unsigned int	fd){
	(void)fd;
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
		std::cerr << this->getNickname() << " don't have permission to change topic." << std::endl;
		return ;
	}
	//else
		//implementer : Modifier ou afficher le thème du channel
}
