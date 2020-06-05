#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include "list.h"
#include "file.h"
#include "data.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;

const unsigned MAX_CONNECTION = 100;
const char* SERVER_ADDR = "127.0.0.1";
const unsigned SERVER_PORT = 1111;

const unsigned short competence_matrix[7][4] =
{
	{1, 0, 0, 0},
	{1, 2, 0, 0},
	{0, 3, 4, 0},
	{2, 3, 5, 6},
	{2, 4, 6, 7},
	{2, 5, 6, 8},
	{4, 6, 8, 10}
};


class Server
{
private:
	WSAData wsaData; //переместить в init
	SOCKADDR_IN addr;
	size_t addr_size;
	SOCKET sock_listen;
	List<User> users;
	List<User> admins;
	List<Goal> goals;
	
	File<User> fuser;
	File<Goal> fgoal;

	friend void client_handler();
	friend void single_connection_handle(int index);

	string * get_argv(string);

public:
	Connection connection[MAX_CONNECTION];
	int connection_count;

	Server() : connection_count(0) {}

	void init_library()
	{
		if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
		{
			std::cout << "STARTUP Error" << std::endl; //EXCEPTION
			exit(1);
		}
	}
	void set_addr(const char* ip_addr, unsigned short port, ip_version ip_v)
	{
		addr.sin_addr.s_addr = inet_addr(ip_addr);
		addr.sin_port = htons(port);
		addr.sin_family = ip_v;
		addr_size = sizeof(addr);
		sock_listen = socket(ip_v, SOCK_STREAM, NULL);
	}

	void start_up()
	{
		bind(sock_listen, (SOCKADDR*)& addr, addr_size);
		listen(sock_listen, MAX_CONNECTION);
	}

	void start_client_handler() // переделать под список
	{
		void client_handler();
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)client_handler, NULL, NULL, NULL);
	}

	void open_data()
	{
		fgoal.open_goals(goals, "goals.txt");
		fuser.OpenLogPass(users, "users.txt");
		fuser.OpenLogPass(admins, "admins.txt");
	}

	void save_data()
	{
		fgoal.save_goals(goals, "goals.txt");
		fuser.SaveUsers(users, "users.txt");
		fuser.SaveUsers(admins, "admins.txt");
	}

	static bool find(User user, List<User>& list)
	{
		for (int i = 0; i < list.size(); ++i)
		{
			if (list[i].login == user.login && list[i].pass == user.pass)
				return true;
		}
		return false;
	}

	static int find(string login, List<User>& list)
	{
		for (int i = 0; i < list.size(); ++i)
		{
			if (list[i].login == login)
				return i;
		}
		return -1;
	}

	bool recv_packet(unsigned);
	void send_packet(unsigned, packet_type, char *data, size_t);
};

bool Server::recv_packet(unsigned socket_index)
{
	packet_type pack_type;
	recv(connection[socket_index].sock, (char*)& pack_type, sizeof(packet_type), NULL);
	string data_str;
	size_t data_size;
	char* data;
	Authorization_type type;
	int result;
	int size;

	string* argv = NULL;
	switch (pack_type)
	{
	case authorization:
		recv(connection[socket_index].sock, (char*)& data_size, sizeof(int), NULL);

		data = new char[data_size];
		recv(connection[socket_index].sock, data, data_size, NULL);
		data_str = data;

		argv = get_argv(data_str);
		type = not_authorized;
		if (find(User(argv[0], argv[1]), users))
			type = user;
		else if (find(User(argv[0], argv[1]), admins))
			type = admin;

		if (type != not_authorized)
		{
			connection[socket_index].authorization_type = type;
			connection[socket_index].name = argv[0];
		}
		break;

	case authorization_status:
		send_packet(socket_index, authorization_status, (char*)&(connection[socket_index].authorization_type), sizeof(Authorization_type));
		break;

	case get_post:
		result = find(connection[socket_index].name, users);
		send_packet(socket_index, get_post, (char *)&users[result].post, sizeof(int));
		break;

	case get_degree:
		result = find(connection[socket_index].name, users);
		send_packet(socket_index, get_degree, (char*)& users[result].degree, sizeof(int));
		break;

	case set_post:
		int post;
		recv(connection[socket_index].sock, (char*)&data_size, sizeof(size_t), NULL);
		recv(connection[socket_index].sock, (char*)& post, data_size, NULL);
		result = find(connection[socket_index].name, users);
		users[result].post = post;
		break;

	case set_degree:
		int degree;
		recv(connection[socket_index].sock, (char*)& data_size, sizeof(size_t), NULL);
		recv(connection[socket_index].sock, (char*)& degree, data_size, NULL);
		result = find(connection[socket_index].name, users);
		users[result].degree = degree;
		break;

	case add_user:
		recv(connection[socket_index].sock, (char*)& data_size, sizeof(int), NULL);
		data = new char[data_size];
		recv(connection[socket_index].sock, data, data_size, NULL);
		data_str = data;
		argv = get_argv(data_str);
		users.push_tail(User(argv[0], argv[1], -1, -1));
		break;

	case get_table_size:
		size = goals.size();
		send_packet(socket_index, get_table_size, (char*)& size, sizeof(int));
		break;

	case get_table:
		for (int i = 0; i < goals.size(); ++i)
		{
			User_goal tmp(goals[i]);
			send_packet(socket_index, get_table, (char*)& tmp, sizeof(User_goal));
		}
		break;

	case set_mark:
		double mark;
		int competence;
		result = find(connection[socket_index].name, users);
		competence = competence_matrix[users[result].post][users[result].degree];

		recv(connection[socket_index].sock, (char*)& size, sizeof(int), NULL);
		data = new char[size];
		recv(connection[socket_index].sock, data, size, NULL);
		recv(connection[socket_index].sock, (char*)& pack_type, sizeof(packet_type), NULL);
		recv(connection[socket_index].sock, (char*)& size, sizeof(int), NULL);
		recv(connection[socket_index].sock, (char*)& mark, size, NULL);
		bool set_mark_status;
		for (int i = 0; i < goals.size(); ++i)
		{
			set_mark_status = false;
			if (goals[i].name == data)
			{
				for (int j = 0; j < goals[i].amount; ++j)
					if (goals[i].mark[j].user_name == connection[socket_index].name)
					{
						goals[i].mark[j].mark = mark;
						goals[i].mark[j].competence = competence;
						set_mark_status = true;
					}
				if (!set_mark_status)
				{
					goals[i].mark[goals[i].amount].user_name = connection[socket_index].name;
					goals[i].mark[goals[i].amount].mark = mark;
					goals[i].mark[goals[i].amount].competence = competence;
					++(goals[i].amount);
				}
				break;
			}
		}
		break;

	case set_goal:
		recv(connection[socket_index].sock, (char*)&size, sizeof(int), NULL);
		data = new char[size];
		recv(connection[socket_index].sock, data, size, NULL);
		goals.push_tail(Goal(data));
		break;

	case remv_goal:
		recv(connection[socket_index].sock, (char*)& size, sizeof(int), NULL);
		data = new char[size];
		recv(connection[socket_index].sock, data, size, NULL);
		for (int i = 0; i < goals.size(); ++i)
			if (goals[i].name == data)
			{
				goals.pop(i);
				break;
			}
		break;

	case get_goal_info:
		recv(connection[socket_index].sock, (char*)& size, sizeof(int), NULL);
		data = new char[size];
		recv(connection[socket_index].sock, data, size, NULL);
		result = -1;
		for (int i = 0; i < goals.size(); ++i)
			if (goals[i].name == data)
			{
				result = i;
				break;
			}
		if (result == -1)
			exit(4);
		send_packet(socket_index, get_goal_info, (char*)&(goals[result].amount), sizeof(int));
		for (int i = 0; i < goals[result].amount; ++i)
		{
			data_str = goals[result].mark[i].user_name;
			size = goals[result].mark[i].user_name.size() + 1;
			send_packet(socket_index, get_goal_info, (char*)& (goals[result].mark[i].mark), sizeof(double));
			send_packet(socket_index, get_goal_info, (char*)& (goals[result].mark[i].competence), sizeof(int));
			send(connection[socket_index].sock, (char*)& size, sizeof(int), NULL);
			send(connection[socket_index].sock, data_str.c_str(), size, NULL);
		}
		break;

	case get_users_size:
		size_t str_size;
		size = users.size();
		send(connection[socket_index].sock, (char*)& size, sizeof(int), NULL);

		for (int i = 0; i < size; ++i)
		{
			str_size = users[i].login.size() + 1;
			send(connection[socket_index].sock, (char*)& str_size, sizeof(int), NULL);
			send(connection[socket_index].sock, (char*)(users[i].login.c_str()), str_size, NULL);
			str_size = users[i].pass.size() + 1;
			send(connection[socket_index].sock, (char*)& str_size, sizeof(int), NULL);
			send(connection[socket_index].sock, (char*)(users[i].pass.c_str()), str_size, NULL);
		}
		break;

	case rmv_user:
		recv(connection[socket_index].sock, (char*)& size, sizeof(int), NULL);
		data = new char[size];
		recv(connection[socket_index].sock, data, size, NULL);
		for (int i = 0; i < users.size(); ++i)
			if (users[i].login == data)
			{
				users.pop(i);
				break;
			}
		break;

	case close_connection:
		connection[socket_index].sock = 0;
		break;
	default:
		return false;
	}
	return true;
}

void Server::send_packet(unsigned socket_index, packet_type type, char* data, size_t size)
{
	send(connection[socket_index].sock, (char*)& type, sizeof(packet_type), NULL);
	send(connection[socket_index].sock, (char*)& size, sizeof(size_t), NULL);
	send(connection[socket_index].sock, data, size, NULL);
}

string * Server::get_argv(string data)
{
	char* buf = new char[data.length()];
	size_t argc = 1;
	for (int i = 0; i < data.length(); ++i)
		if (data[i] == '|')
			argc++;
	string *argv = new string[argc];

	argc = 0;
	size_t buf_pos = 0;
	for (int i = 0; i < data.length(); ++i)
	{
		if (data[i] == '|')
		{
			buf[buf_pos] = '\0';
			argv[argc++] = buf;
			buf_pos = 0;
			continue;
		}
		buf[buf_pos++] = data[i];
	}
	buf[buf_pos] = '\0';
	argv[argc] = buf;
	return argv;
}



Server server;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");
	server.init_library();
	server.set_addr(SERVER_ADDR, SERVER_PORT, ip_v4);
	server.start_up();
	server.open_data();
	server.start_client_handler();

	char c;
	while (true)
	{
		system("cls");
		cout.setf(ios::left);
		cout << "  Адрес сервера: " << SERVER_ADDR << endl;
		cout << "  Порт сервера: " << SERVER_PORT << endl << endl;
		cout << "  Список подключений:" << endl;
		cout << " +-------------------------+------------------+--------+" << endl;
		cout << " | Имя пользователя        | Уровень доступа  | Сокет  |" << endl;
		cout << " +-------------------------+------------------+--------+" << endl;
		for (int i = 0; i < server.connection_count; ++i)
		{
			if (server.connection[i].sock != 0)
			{
				cout << " | "
					<< setw(23) << (server.connection[i].name == "" ? "NONAME" : server.connection[i].name) << " | "
					<< setw(16) << (server.connection[i].authorization_type == admin ? "ADMIN" : (server.connection[i].authorization_type == user ? "USER" : "NOT_AUTHORIZED")) << " | "
					<< setw(6) << server.connection[i].sock << " |" << endl;
				cout << " +-------------------------+------------------+--------+" << endl;
			}
		}
		cout << endl << " Нажмите esc для отключения сервера, \n Нажмите любую кнопку для обновления таблицы" << endl;
		c = _getch();
		if (c == 27)
			break;
	}

	server.save_data();
}



void client_handler()  //  переделать под список, костыль с указателем на функцию
{
	for (int i = 0; i < MAX_CONNECTION; ++i)
	{
		server.connection[i].sock = accept(server.sock_listen, (SOCKADDR*)& server.addr, (int*)& server.addr_size);
		if (server.connection[i].sock == 0)
		{
			cout << "connection error" << endl;
			exit(2);
		}
		void single_connection_handle(int index);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)single_connection_handle, (LPVOID)i, NULL, NULL);
		server.connection_count++;
	}
}

void single_connection_handle(int index)  //ОБРАЗЕЦ
{
	bool status = true;
	while (status)
	{
		status = server.recv_packet(index);
	}
}