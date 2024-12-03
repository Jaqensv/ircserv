#include "../includes/Tester.hpp"

void channelTester(Server &server, unsigned int clientFd, std::string channel_name)
{
	BACKLINE;
	std::cout << "##################### TESTER #####################" << std::endl;
	BACKLINE;
	std::cout << "Channels created : " << std::endl;
	server.createChannel(clientFd, channel_name);
	std::cout << "'" << server.getChannel(channel_name).getName() << "'" << " channel created by user " << clientFd << std::endl;
	server.createChannel(clientFd, "new1");
	std::cout << "'" << server.getChannel("new1").getName() << "'" << " channel created by user " << clientFd << std::endl;
	server.createChannel(clientFd, "new2");
	std::cout << "'" << server.getChannel("new2").getName() << "'" << " channel created by user " << clientFd << std::endl;
	BACKLINE;
	std::cout << "This server contains these channels : " << std::endl;
	for (std::vector<Channel*>::const_iterator it = server.getChannels().begin(); it != server.getChannels().end(); ++it)
		std::cout << (*it)->getName() << std::endl;
	BACKLINE;
	std::cout << "In " << server.getChannel(channel_name).getName() << ", we have these users : " << std::endl;
	for (std::map<int, User*>::iterator it = server.getChannel(channel_name).getUsers().begin(); it != server.getChannel(channel_name).getUsers().end(); ++it)
		std::cout << "User name : " << it->second->getUsername() << " | " << "fd : " << it->first << std::endl;
	BACKLINE;
	std::cout << "##################### TESTER #####################" << std::endl;
	BACKLINE;
}
