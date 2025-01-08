	#include "Channel.hpp"

	void 	Channel::kick(Server &server, unsigned int fd, std::string nickname) {
		size_t pos = nickname.find("\r\n");
		if (pos != std::string::npos)
			nickname = nickname.substr(0, pos);
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
			removeInvited(userTarget->getFd());
			revokeOperator(fd, userTarget->getFd());
			userTarget->setMyChannel("");
		}
	}

	void 	Channel::part(unsigned int fd) {

		User* user = getUser(fd);

		if (!user) {
			std::cout << ":You're not on that channel" << std::endl;
			return;
		}
		removeUser(user->getFd());
		removeInvited(user->getFd());
		revokeOperator(fd, user->getFd());
		user->setMyChannel("");
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
			size_t pos = new_string.find("\r\n");
			if (pos != std::string::npos)
				new_string = new_string.substr(0, pos);
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



// INVITE message
//      Command: INVITE
//   Parameters: <nickname> <channel>
// The INVITE command is used to invite a user to a channel. The parameter <nickname> is the nickname of the person to be invited to the target channel <channel>.

// The target channel SHOULD exist (at least one user is on it). Otherwise, the server SHOULD reject the command with the ERR_NOSUCHCHANNEL numeric.

// Only members of the channel are allowed to invite other users. Otherwise, the server MUST reject the command with the ERR_NOTONCHANNEL numeric.

// Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric. In particular, they SHOULD reject it when the channel has invite-only mode set, and the user is not a channel operator.

// If the user is already on the target channel, the server MUST reject the command with the ERR_USERONCHANNEL numeric.

// When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer, and an INVITE message, with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.

// Command Examples:

//   INVITE Wiz #foo_bar    ; Invite Wiz to #foo_bar
// Message Examples:

//   :dan-!d@localhost INVITE Wiz #test    ; dan- has invited Wiz
//                                         to the channel #test