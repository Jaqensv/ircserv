#include "../includes/Tester.hpp"

// void channelTester(Server &server, unsigned int clientFd, std::string channel_name)
// {
// 	BACKLINE;
// 	std::cout << "##################### TESTER #####################" << std::endl;
// 	BACKLINE;

// 	std::cout << "Channels created : " << std::endl;
// 	if (server.isChannel(channel_name) == false) {
// 		server.createChannel(clientFd, channel_name);
// 		std::cout << "'" << server.getChannel(channel_name).getName() << "'" << " channel created by user " << clientFd << std::endl;
// 	}
// 	if (server.isChannel("new1") == false) {
// 		server.createChannel(clientFd, "new1");
// 		std::cout << "'" << server.getChannel("new1").getName() << "'" << " channel created by user " << clientFd << std::endl;
// 	}
// 	if (server.isChannel("new2") == false) {
// 		server.createChannel(clientFd, "new2");
// 		std::cout << "'" << server.getChannel("new2").getName() << "'" << " channel created by user " << clientFd << std::endl;
// 	}
// 	BACKLINE;
// 	std::cout << "This server contains these channels : " << std::endl;
// 	for (std::vector<Channel*>::const_iterator it = server.getChannels().begin(); it != server.getChannels().end(); ++it)
// 		std::cout << (*it)->getName() << std::endl;
// 	BACKLINE;
// 	std::cout << "In " << server.getChannel(channel_name).getName() << ", we have these users : " << std::endl;

// 	for (std::map<int, User*>::iterator user_it = server.getChannel(channel_name).getUsers().begin(); user_it != server.getChannel(channel_name).getUsers().end(); ++user_it)
// 		std::cout << "User name : " << user_it->second->getUsername() << " | " << "fd : " << user_it->first << std::endl;
// 	BACKLINE;
// 	std::cout << "In " << server.getChannel(channel_name).getName() << ", we have these operators : " << std::endl;
// 	for (std::map<int, User*>::iterator oper_it = server.getChannel(channel_name).getOpers().begin(); oper_it != server.getChannel(channel_name).getOpers().end(); ++oper_it)
// 		std::cout << "User name : " << oper_it->second->getUsername() << " | " << "fd : " << oper_it->first << std::endl;

// 	BACKLINE;
// 	std::cout << "We add one user who becomes an operator" << std::endl;
// 	server.getChannel(channel_name).addUser(clientFd);
// 	server.getChannel(channel_name).addOperator(clientFd);
// 	std::cout << "He tries to change the topic into " << channel_name << std::endl;
// 	server.getChannel(channel_name).setTopic(clientFd, server.getChannel(channel_name).getName(), "NOUVEAU TOPIC WESH");
// 	BACKLINE;
// 	std::cout << "In " << server.getChannel(channel_name).getName() << ", we have these users : " << std::endl;

// 	for (std::map<int, User*>::iterator user_it = server.getChannel(channel_name).getUsers().begin(); user_it != server.getChannel(channel_name).getUsers().end(); ++user_it)
// 		std::cout << "User name : " << user_it->second->getUsername() << " | " << "fd : " << user_it->first << std::endl;
// 	BACKLINE;
// 	std::cout << "In " << server.getChannel(channel_name).getName() << ", we have these operators : " << std::endl;
// 	for (std::map<int, User*>::iterator oper_it = server.getChannel(channel_name).getOpers().begin(); oper_it != server.getChannel(channel_name).getOpers().end(); ++oper_it)
// 		std::cout << "User name : " << oper_it->second->getUsername() << " | " << "fd : " << oper_it->first << std::endl;
// 	BACKLINE;
// 	if (server.getChannel(channel_name).getOpers().size() > 1) {
// 		std::cout << "We revoke the rights of the first operator" << std::endl;
// 		std::map<int, User*>::iterator user_it = server.getChannel(channel_name).getOpers().begin();
// 		server.getChannel(channel_name).revokeOperator(clientFd, user_it->first);
// 		std::cout << "In " << server.getChannel(channel_name).getName() << ", we have these operators : " << std::endl;
// 		for (std::map<int, User*>::iterator oper_it = server.getChannel(channel_name).getOpers().begin(); oper_it != server.getChannel(channel_name).getOpers().end(); ++oper_it)
// 			std::cout << "User name : " << oper_it->second->getUsername() << " | " << "fd : " << oper_it->first << std::endl;
// 	}
// 	BACKLINE;
// 	std::cout << "##################### TESTER #####################" << std::endl;
// 	BACKLINE;
// }


// void channelTesterLite(std::string channel_name) {

// 	Server &server = server.getInstance();
// 	BACKLINE;
// 	//std::cout << "CHANNEL NAME " << channel_name << std::endl;
// 	std::cout << "##################### LITE TESTER #####################" << std::endl;
// 	BACKLINE;
// 	if (server.getChannel(channel_name).getUser(6) == NULL) {
// 		std::cout << "We add an user and name him 'Jean'" << std::endl;
// 		server.getChannel(channel_name).addUser(server, 6);
// 		server.getChannel(channel_name).getUser(6)->setNickname("Jean");
// 	}
// 	for (std::map<int, User*>::iterator it = server.getUsers().begin(); it != server.getUsers().end(); ++it)
// 		std::cout << "Array User name : " << it->second->getNickname() << " | " << "fd : " << it->first << std::endl;
// 	std::cout << "In " << server.getChannel(channel_name).getName() << ", we have these users : " << std::endl;
// 	for (std::map<int, User*>::iterator user_it = server.getChannel(channel_name).getUsers().begin(); user_it != server.getChannel(channel_name).getUsers().end(); ++user_it)
// 		std::cout << "User name : " << user_it->second->getNickname() << " | " << "fd : " << user_it->first << std::endl;
// 	BACKLINE;
// 	std::cout << "##################### LITE TESTER #####################" << std::endl;
// }

void channelTopicTester(std::string channel_name) {
	Server &server = server.getInstance();
	if (channel_name[0] == '#')
		channel_name.erase(0, 1);
	Channel &channel = server.getChannel(channel_name);
	std::cout << "Topic: " << channel.getTopic() << std::endl;
}