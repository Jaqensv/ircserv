#include <iostream>
#include <string>
#include "../includes/Operator.hpp"
#include "../includes/Server.hpp"

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

