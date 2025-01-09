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
	size_t pos = server._arrayParams.params[0].find("\r\n");
	if (pos != std::string::npos)
		server._arrayParams.params[0] = server._arrayParams.params[0].substr(0, pos);
	if(isChannel(server._arrayParams.params[0])) {
		Channel &chan = getChannel(server._arrayParams.params[0]);
		if (chan.isInvOnly() == true) {

			if (server.getChannel(server._arrayParams.params[0]).isInvited(clientFd) == false) {
				std::cout << "ERROR :channel is invite only." << std::endl;
				return;
			}
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
		if (chan.getUser(clientFd) != NULL) {
			std::cout << "ERROR :User is already on that channel." << std::endl;
			return;
		}
		chan.addUser(server, clientFd);
	}
	else {
		createChannel(server, clientFd, server._arrayParams.params[0]);
		std::cout << "Channel " << server.getChannel(server._arrayParams.params[0]).getName() << " created by " << server.getUser(clientFd).getNickname() << std::endl;
	}
	server.getUser(clientFd).getMyChannels().push_back(server._arrayParams.params[0]);
}

void	Server::invite(std::string nickname, std::string channel) {
	Server	&server = Server::getInstance();
	unsigned int user_fd = server.getTargetUserFd(nickname);

	if (server.isUser(user_fd) == true) {
		if (channel[0] != '#') {
			std::cout << "ERROR CHAN : First param after command needs to be a #channel." << std::endl;
			return;
		}
		channel.erase(0, 1);
		size_t pos = channel.find("\r\n");
		if (pos != std::string::npos)
			channel = channel.substr(0, pos);
		if (server.isChannel(channel) == true) {
			User user = server.getUser(user_fd);
			user.getMyChannels().push_back(channel); // a virer ???
			server.getChannel(channel).getInvited().insert(std::make_pair(user_fd, &user));
		}
		else
			std::cout << "Error: channel " << channel << " doesn't exist" << std::endl;
	}
	else
		std::cout << "Error: user " << nickname << " doesn't exist" << std::endl;
}

//:User is already on that channel