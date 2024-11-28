#pragma once
#include <iostream>
#include <string>
#include "User.hpp"

class Oper : public User{

	public :
	//Constructor
		Oper(int fd) : User(fd){}

	//Member method Operator
		void	kick(unsigned int fd);
		void	invite(unsigned int	fd);
		void	changePassw(std::string passw);
		void	deletePassw();
		void	useTopic();

	private :
	//Member variable
		bool	_canTopic;

};
