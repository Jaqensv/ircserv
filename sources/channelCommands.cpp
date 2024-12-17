	#include "Channel.hpp"

	void Channel::kick(Server &server, unsigned int fd, std::string nickname) {
		if(nickname.find("\r\n"))
				nickname = nickname.substr(0, nickname.size() - 2);
		User* user = getUser(fd);
		User* userTarget = getUser(server.getTargetUserFd(nickname));
		if (!isOperator(fd))
			std::cout << user->getUsername() << " doesn't have the rights to kick" << std::endl;
		else {
			if (userTarget == NULL) {
				std::cout << "User " << nickname << " not found" << std::endl;
				return;
			}
			std::string message = "KICK " + _name + " " + userTarget->getNickname() + " :You have been kicked by " + user->getNickname() + "\r\n";
			ssize_t bytesSent = send(userTarget->getFd(), message.c_str(), message.size(), 0);
			if (bytesSent == -1) {
				std::cerr << "ERROR KICK : Failed to send message to client " << userTarget->getFd() << std::endl;
				return;
			}
			std::cout << user->getUsername() << " has kicked " << userTarget->getUsername() << std::endl;
			removeUser(userTarget->getFd());
			userTarget->setMyChannel("");
		}
	}