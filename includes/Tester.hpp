#pragma once
#include "Channel.hpp"
#include "Operator.hpp"
#include "Server.hpp"
#include "User.hpp"
#define BACKLINE std::cout << '\n'

void channelTester(Server &server, unsigned int clientFd, std::string channel_name);