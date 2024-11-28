#pragma once
#include <iostream>
#include <string>
#include "User.hpp"

class Oper : public User{

	public :
	//Constructor
		Oper(int fd) : User(fd){}

	//Member method Operator
		void	kick(unsigned int	fd);
		void	invite(unsigned int	fd);
		void	changePassw(std::string passw);
		void	deletePassw();
		void	useTopic();
		//void	addPrivilege(std::string channel_name);

	private :
	//Member variable
		bool					_canTopic;
		//std::vector<Channel> 	_pChannels; // liste des channels sur lesquels l'user est op

};
