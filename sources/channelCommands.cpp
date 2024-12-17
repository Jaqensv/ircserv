	#include "Channel.hpp"

	void 	Channel::kick(Server &server, unsigned int fd, std::string nickname) {
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

	void	Channel::setTopic(unsigned int fd, std::vector<std::string> topic) {
			std::string new_string;
			std::vector<std::string>::iterator it = topic.begin();
			std::advance(it, 1);
			for (; it != topic.end(); ++it) {
				new_string += *it + " ";
			}
			if (new_string.size() > 1)
				new_string.erase(new_string.size() - 2, 1);
			if (new_string.find("\r\n")) {
				new_string = new_string.substr(0, new_string.size() - 2);
			}
			User* user = _users.find(fd)->second;
			if (user) {
				if (_canTopic == true) {
					std::cout << "true" << std::endl;
					_topic = new_string;
					std::cout << user->getUsername() << " has changed the topic: " << _topic << std::endl;
				}
				else {
					std::cout << "false" << std::endl;
					if (isOperator(fd)) {
						_topic = new_string;
						std::cout << user->getUsername() << " has changed the topic: " << _topic << std::endl;
					}
					else
						std::cout << user->getUsername() <<  " doesn't have the rights to change the topic" << std::endl;
				}
			}
			else
				std::cerr << "Error: user doesn't exist" << std::endl;
	}

		void	Channel::setTopic(unsigned int fd) {
			std::string new_string = "";
			User* user = _users.find(fd)->second;
			if (user) {
				if (_canTopic == true) {
					std::cout << "true" << std::endl;
					_topic = new_string;
					std::cout << user->getUsername() << " has changed the topic: " << _topic << std::endl;
				}
				else {
					std::cout << "false" << std::endl;
					if (isOperator(fd)) {
						_topic = new_string;
						std::cout << user->getUsername() << " has changed the topic: " << _topic << std::endl;
					}
					else
						std::cout << user->getUsername() <<  " doesn't have the rights to change the topic" << std::endl;
				}
			}
			else
				std::cerr << "Error: user doesn't exist" << std::endl;
	}

