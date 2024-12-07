#pragma once
#include <iostream>
#include <string>

class User{

	public :

	//Constructor & Destructor
		User(User const &copy);
		User(unsigned int fd);
		~User();

	//Surcharge operator
		User	&operator=(User const &other);

	// Getter
		int			getFd();
		std::string	getNickname();
		std::string	getUsername();

	//Setter
		void	setFd(unsigned int fd);
		void	setNickname(std::string nickname);
		void	setUsername(std::string username);


	private :
		User();
	//Variable member
		unsigned int	_fd;
		std::string		_nickname;
		std::string		_username;

};
