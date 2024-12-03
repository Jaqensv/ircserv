#include "Tester.hpp"

void channelTester(Server &server, unsigned int clientFd, std::string channel_name) {
	server.createChannel(clientFd, "new");
	server.createChannel(clientFd, "new1");
	server.createChannel(clientFd, "new2");
	std::cout << "false :" << std::endl;
	std::cout << server.getChannel("test") << std::endl;
	std::cout << "true :" << std::endl;
	std::cout << server.getChannel("new2") << std::endl;
}