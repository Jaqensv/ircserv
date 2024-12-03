#pragma once
#include "includes/Channel.hpp"
#include "includes/Operator.hpp"
#include "includes/Server.hpp"
#include "includes/User.hpp"

class Tester : public Channel, public Oper, public Server, public User {

public : 

	Tester(){};
	~Tester(){};
	
};

void channelTester(Server &server, unsigned int clientFd, std::string channel_name);