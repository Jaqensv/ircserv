#include <iostream>
#include <string>
#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"

void	Server::join(int clientFd){

	Server	&server = Server::getInstance();
	if (server._arrayParams.params.empty())
	{
		std::cout << "ERROR: No channel specified" << std::endl;
		return;
	}
	if (server._arrayParams.params[0][0] != '#')
	{
		std::cout << "ERROR CHAN : First param after command needs to be a #channel." << std::endl;
		return;
	}
	server._arrayParams.params[0].erase(0, 1);
	if(server._arrayParams.params[0].find("\r\n"))
			server._arrayParams.params[0] = server._arrayParams.params[0].substr(0, server._arrayParams.params[0].size() - 2);
	if(isChannel(server._arrayParams.params[0])) {
		Channel &chan = getChannel(server._arrayParams.params[0]);
		if (chan.isInvOnly() == true) {
			std::cout << "ERROR :channel is invite only." << std::endl;
			return;
		}
		if (chan.isLimitMode() == true) {
			if (chan.getUsers().size() == chan.getLimit()) {
				std::cout << "ERROR : channel is full." << std::endl;
				return;
			}
		}
		if (chan.isKeyMode() == true) {
			if (server._arrayParams.params.size() == 2) {
				if (server._arrayParams.params[1] != chan.getKey()) {
					std::cout << "ERROR : wrong password." << std::endl;
					return;
				}
			}
			else {
				std::cout << "ERROR : need password." << std::endl;
				return;
			}
		}
		chan.addUser(server, clientFd);
		if(server.getUser(clientFd).getMyChannel() != "")
			server.getChannel(server.getUser(clientFd).getMyChannel()).getUsers().erase(server.getChannel(server.getUser(clientFd).getMyChannel()).getUsers().find(clientFd));
		server.getUser(clientFd).setMyChannel(chan.getName());
	}
	else {
		createChannel(server, clientFd, server._arrayParams.params[0]);
		std::cout << "Channel " << server.getChannel(server._arrayParams.params[0]).getName() << " created by " << server.getUser(clientFd).getNickname() << std::endl;
		if(server.getUser(clientFd).getMyChannel() != "")
			server.getChannel(server.getUser(clientFd).getMyChannel()).getUsers().erase(server.getChannel(server.getUser(clientFd).getMyChannel()).getUsers().find(clientFd));
		server.getUser(clientFd).setMyChannel(server.getChannel(server._arrayParams.params[0]).getName());
	}
	server.getUser(clientFd).getMyChannels().push_back(server.getUser(clientFd).getMyChannel());

}
