#pragma once
#include <iostream>
#include <string>
#include "User.hpp"

class Channel;

class Oper : public User{

	public :
	//Constructor
		Oper(int fd) : User(fd){}

	//Member method Operator
		void	kick(unsigned int fd, Channel &channel);
		void	invite(unsigned int fd, User &user, Channel &channel);
		void	changePassw(std::string passw);
		void	deletePassw();
		void	useTopic();

	private :
	//Member variable
		bool					_canTopic;

};
