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
	std::map<int, User*>::iterator user_it = server.getChannel(channel_name).getUsers().begin();
	for (; user_it != server.getChannel(channel_name).getUsers().end(); ++user_it)
		std::cout << "User name : " << user_it->second->getUsername() << " | " << "fd : " << user_it->first << std::endl;
	BACKLINE;
	std::cout << "In " << server.getChannel(channel_name).getName() << ", we have these operators : " << std::endl;
	std::map<int, Oper*>::iterator oper_it = server.getChannel(channel_name).getOpers().begin();
	for (; oper_it != server.getChannel(channel_name).getOpers().end(); ++oper_it) {
		unsigned fd = oper_it->first;
		std::string user_name = NULL;
		for (std::map<int, User*>::iterator user_it = server.getChannel(channel_name).getUsers().begin(); user_it != server.getChannel(channel_name).getUsers().end(); ++user_it) {
			if (fd == user_it->first)
				std::cout << "User name : " << user_it->second->getUsername() << " | " << "fd : " << oper_it->first << std::endl;
		}
	}
	BACKLINE;
	// unsigned int new_fd = clientFd++;
	// std::cout << "We add one user who becomes an operator" << std::endl;
	// server.getChannel(channel_name).addUser(new_fd);
	// server.getChannel(channel_name).addOperator(new_fd);
	std::cout << "##################### TESTER #####################" << std::endl;
	BACKLINE;
}
 