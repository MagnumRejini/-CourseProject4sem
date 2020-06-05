#pragma once
#include <WinSock2.h>
#include <string>
#include "list.h"
#include <string.h>

enum ip_version
{
	ip_v4 = AF_INET,
	ip_v6 = AF_INET6
};

enum packet_type
{
	authorization,
	authorization_status,
	set_degree,
	get_degree,
	set_post,
	get_post,
	add_user,
	get_table_size,
	get_table,
	set_mark,
	set_goal, 
	remv_goal,
	get_goal_info,
	get_users_size,
	rmv_user,
	close_connection
};

enum Authorization_type
{
	not_authorized,
	user,
	admin,
	root
};

struct Connection
{
	SOCKET sock;
	Authorization_type authorization_type;
	string name;

	Connection() : sock(0), authorization_type(not_authorized), name("") {}
	Connection(Connection& conn)
	{
		sock = conn.sock;
		authorization_type = conn.authorization_type;
		name = conn.name;
	}
};

struct Mark
{
	string user_name;
	double mark;
	int competence;

	Mark() : user_name(""), mark(0), competence(0) {}
	Mark(string user_name, double mark, int competence) : user_name(user_name), mark(mark), competence(competence) {}
};

struct Goal
{
	string name;
	Mark mark[20];
	int amount;

	Goal() : name(""), mark(), amount(0) {}
	Goal(char* name) : name(name), amount(0), mark() {}
	Goal(string name, Mark* ptr, int size) : name(name), amount(size)
	{
		for (int i = 0; i < size; ++i)
		{
			mark[i].user_name = ptr[i].user_name;
			mark[i].mark = ptr[i].mark;
			mark[i].competence = ptr[i].competence;
		}
	}
};

struct User_goal
{
	char goal_name[100];
	double goal_weight;
	double average_competence;
	double max_mark;
	double min_mark;

	User_goal(Goal goal)
	{
		strcpy_s(goal_name, 100, goal.name.c_str());
		if (goal.amount == 0)
		{
			goal_weight = -1;
			average_competence = 0;
			max_mark = 0;
			min_mark = 0;
			return;
		}
		double sum_comp = 0;
		max_mark = 0;
		min_mark = 1;
		for (int i = 0; i < goal.amount; ++i)
		{
			sum_comp += goal.mark[i].competence;
			max_mark = (max_mark > goal.mark[i].mark ? max_mark : goal.mark[i].mark);
			min_mark = (min_mark < goal.mark[i].mark ? min_mark : goal.mark[i].mark);	
		}
		average_competence = sum_comp / (double)goal.amount;
		goal_weight = 0;
		for (int i = 0; i < goal.amount; ++i)
			goal_weight += goal.mark[i].mark * (goal.mark[i].competence / sum_comp);
	}
};