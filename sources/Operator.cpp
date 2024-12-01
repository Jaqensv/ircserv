#include <iostream>
#include <string>
#include "../includes/Operator.hpp"
#include "../includes/Server.hpp"

void	Oper::kick(std::string channel, std::string user, Server &server){
	std::vector<Channel> chan;
		if (channel.empty() || user.empty())
			std::cerr << "Error: invalid command" << std::endl;
		else {
			if (std::find(server.getChannels().begin(), server.getChannels().end(), channel) == server.getChannels().end())
				std::cerr << channel << " doesn't exist" << std::endl;
			if (std::find(server.getUsers().begin(), server.getUsers().end(), user) == server.getUsers().end())
				std::cerr << user << " doesn't exist" << std::endl;
		}
			channel.removeUser(fd);}

void	Oper::invite(unsigned int fd, User &user, Channel &channel){channel.addUser(fd, user);}

void	Oper::topic(Channel &channel){
	if (_canTopic == true)
		std::cout << channel.getName() << " : " << channel.getTopic() << std::endl;
	else
		std::cerr << "Error: needs rights to topic" << std::endl;
}

void	Oper::topic(Channel &channel, std::string topic){
	if (_canTopic == true) {
		channel.setTopic(topic);
		std::cout << channel.getName() << " topic modified : " << channel.getTopic() << std::endl;
	}
	else
		std::cerr << "Error: needs rights to topic" << std::endl;
}

void	Oper::changePassw(std::string passw){
	Server	&server = Server::getInstance();
	server.setPassw(passw);
}

void	Oper::deletePassw(){
	Server	&server = Server::getInstance();
	server.setPassw("");
	server.setNeedPasswFalse();
}

