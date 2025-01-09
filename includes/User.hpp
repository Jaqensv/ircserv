#pragma once
#include <iostream>
#include <string>
#include "../includes/Channel.hpp"

class Channel;

class User{

	public :

	//Constructor & Destructor
		User(User const &copy);
		User(unsigned int fd);
		~User();

	//Surcharge operator
		User	&operator=(User const &other);

	// Getter
		int							getFd();
		std::string					getNickname();
		std::string					getUsername();
		std::string					getBuffer();
		std::string					getMyChannel();
		std::vector<std::string>	&getMyChannels();
		int 						findChannelIndex(std::string myChannel);
	//Setter
		void	setFd(unsigned int fd);
		void	setNickname(std::string nickname);
		void	setUsername(std::string username);
		void	setBuffer(std::string buffer);
		void	setMyChannel(std::string myChannel);


	private :
		User();
	//Variable member
		unsigned int				_fd;
		std::string					_nickname;
		std::string					_username;
		std::string					_buffer;
		std::string					_myChannel;
		std::vector<std::string>	_myChannels;

};
