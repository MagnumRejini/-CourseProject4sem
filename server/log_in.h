#pragma once

#include <string>
#include <conio.h>
#include "list.h"

class User
{
public:
	std::string login;
	std::string pass;
	int post;
	int degree;

	User(std::string login, std::string pass, int post, int degree)
	{
		this->login = login;
		this->pass = pass;
		this->post = post;
		this->degree = degree;
	}

	User(std::string login, std::string pass)
	{
		this->login = login;
		this->pass = pass;
		this->post = 0;
		this->degree = 1;
	}

	User() : login(""), pass(""), post(-1), degree(-1) {}
};