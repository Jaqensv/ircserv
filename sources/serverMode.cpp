#include "Server.hpp"
#include <string>

//if params[1] doesn't exist  RPL_CHANNELMODEIS (324)
bool	Server::modeCmdParsing(std::vector<std::string> &params, unsigned int myfd) {
	Server	&server = Server::getInstance();
	std::string	&chanName = params[0];

	if (chanName[0] == '#') {
		chanName.erase(0, 1);
	} else {
		std::cout << "Command Mode allow only channel in target with '#' prefix" << std::endl;
		return false;
	}
	size_t pos = chanName.find("\r\n");
	if (pos != std::string::npos)
		chanName = chanName.substr(0, pos);
	if (isChannel(chanName) == false) {
		// ERR_NOSUCHCHANNEL (403)
		std::cerr << "ERROR: Channel doesn't exist" << std::endl;
		return false;
	}
	Channel	&chan = getChannel(chanName);
	if (chan.isOperator(myfd) == false) {
		std::cerr << "ERROR: You are not an operator" << std::endl;
		return false;
	}
	if (params.size() == 1) {
		std::cout << "ERROR: not enougth arguments" << std::endl;
		return true;
	}
	pos = params[1].find("\r\n");
	if (pos != std::string::npos)
		params[1] = params[1].substr(0, pos);
	if (params[1][0] != '+' && params[1][0] != '-') {
		std::cerr << "ERROR: Mode must start with '+' or '-'" << std::endl;
		return false;
	}
	if (params[1].size() != 2) {
		std::cout << "ERROR Bad format: /MODE <#channel> <- or + 'mode letter'>" << std::endl;
		return true;
	}
	bool isAdd = false;
	if (params[1][0] == '+')
		isAdd = true;

	// — t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
	if (params[1][1] == 't') {
		chan.switchCanTopic(isAdd ? true : false);
	}
	// — i : Définir/supprimer le canal sur invitation uniquement
	else if (params[1][1] == 'i') {
		chan.switchInvOnly(isAdd ? true : false);
	}
	// — k : Définir/supprimer la clé du canal (mot de passe)
	else if (params[1][1] == 'k') {
		if (isAdd)
			chan.switchKeyMode(params[2]);
		else
			chan.switchKeyMode();
	}
	// — o : Donner/retirer le privilège de l’opérateur de canal
	else if (params[1][1] == 'o') {
		pos = params[2].find("\r\n");
		if (pos != std::string::npos)
			params[2] = params[2].substr(0, pos);
		User	userTarget = server.getUser(server.getTargetUserFd(params[2]));
		if (isAdd) {
			if (chan.isOperator(userTarget.getFd())) {
				std::cout << params[2] << " is already operator" << std::endl;
			} else {
				chan.addOperator(userTarget.getFd());
				std::cout << params[2] << " has been added to operator" << std::endl;
			}
		} else {
			if (!chan.isOperator(userTarget.getFd())) {
				std::cout << params[2] << " is not operator" << std::endl;
			} else {
				chan.revokeOperator(myfd, userTarget.getFd());
				std::cout << params[2] << " has been revoked from operator" << std::endl;
			}
		}
	}
	// — l : Définir/supprimer la limite d’utilisateurs pour le canal
	else if (params[1][1] == 'l') {
		pos = params[2].find("\r\n");
		if (pos != std::string::npos)
			params[2] = params[2].substr(0, pos);
		if (isAdd) {
			std::stringstream ss(params[2]);
			int num;
			ss >> num;
			chan.switchLimitMode(num);
		} else {
			chan.switchLimitMode();
		}
	}
	return true;
}
