#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
using namespace std;

enum packet_type
{
	authorization,
	authorization_status,
	set_degree,
	get_degree,
	set_post,
	get_post,
	add_usr,
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

enum ip_version
{
	ip_v4 = AF_INET,
	ip_v6 = AF_INET6
};

struct User_goal
{
	char goal_name[100];
	double goal_weight;
	double average_competence;
	double max_mark;
	double min_mark;

	User_goal() {}
};

struct Mark
{
	string user_name;
	double mark;
	int competence;

	Mark() : user_name(""), mark(0), competence(0) {}
	Mark(string user_name, double mark, int competence) : user_name(user_name), mark(mark), competence(competence) {}
};

extern const char* post[7];
extern const char* degree[4];

extern const unsigned short competence_matrix[7][4];

class Client
{
private:
	WSAData wsaData; //переместить в init
	SOCKADDR_IN addr;
	unsigned addr_size;
	SOCKET sock;
public:
	void init_library()
	{
		if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
		{
			cout << "STARTUP Error" << endl; //EXCEPTION
			exit(1);
		}
	}
	void set_addr(const char* ip_addr, unsigned short port, ip_version ip_v)
	{
		addr.sin_addr.s_addr = inet_addr(ip_addr);
		addr.sin_port = htons(port);
		addr.sin_family = ip_v;
		addr_size = sizeof(addr);
		sock = socket(ip_v, SOCK_STREAM, NULL);
	}

	void connect_to_server()
	{
		if (connect(sock, (SOCKADDR*)& addr, sizeof(addr)) != 0)
		{
			cout << "Error: failed connect to server.\n";   //EXCEPTION
			exit(1);
		}
	}

	void send_request(packet_type type)
	{
		send(sock, (char*)& type, sizeof(packet_type), NULL);
	}

	SOCKET get_sock() { return sock; }

	void send_packet(packet_type, string);
	void send_packet(packet_type, char*, size_t);
	void recv_packet(packet_type, string);
	void recv_packet(const packet_type, char*, size_t);
};

