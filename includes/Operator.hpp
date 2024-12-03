#pragma once
#include <iostream>
#include <string>
#include "User.hpp"

class Channel;

class Oper {
	public :
	//Constructor
		Oper(User &user) : _user(user){}

	//Member method Operator
		void	topic(Channel &channel);
		void 	topic(Channel &channel, std::string topic);
		User	&getUser() const {return _user;}

	private :
	//Member variable
		User	&_user;
		bool	_canTopic;
};
