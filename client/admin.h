#pragma once
#include <iostream>
#include "basic_user.h"
#include "client.h"
using namespace std;

extern Client client;

class Admin : public Basic_user
{
private:
	static void print_users();
	static void add_user();
	static void remove_user();
public:
	static void add_goal();
	static void goal_info();
	static void user_manage();
	static void remove_goal();
};

void Admin::print_users()
{
	int size = 0;
	int str_size = 0;
	string login, password;
	char* buf;
	cout << " +------------------+------------------+" << endl;
	cout << " | Логин            | Пароль           |" << endl;
	cout << " +------------------+------------------+" << endl;
	cout.setf(ios::left);
	client.send_request(get_users_size);
	recv(client.get_sock(), (char*)&size, sizeof(int), NULL);
	for (int i = 0; i < size; ++i)
	{
		recv(client.get_sock(), (char*)& str_size, sizeof(int), NULL);
		buf = new char[str_size];
		recv(client.get_sock(), buf, str_size, NULL);
		login = buf;
		recv(client.get_sock(), (char*)& str_size, sizeof(int), NULL);
		delete[] buf;
		buf = new char[str_size];
		recv(client.get_sock(), buf, str_size, NULL);
		password = buf;


		cout << " | " << setw(16) << login << " | " << setw(16) << password << " |" << endl;
		cout << " +------------------+------------------+" << endl;

	}
}

void Admin::add_user()
{
	string login, pass;
	cout << " Введите логин пользователя: ";
	cin >> login;
	cout << " Введите пароль пользователя: ";
	cin >> pass;
	login = login + '|' + pass;
	client.send_packet(add_usr, login);
}

void Admin::remove_user()
{
	string login;
	cout << " Введите логин пользователя: ";
	cin >> login;
	client.send_packet(rmv_user, login);
	cout << " Пользователь удалён";
}

void Admin::add_goal()
{
	cin.sync();
	cin.clear();
	rewind(stdin);
	string goal_name;
	cout << " Введите название цели: ";
	getline(cin, goal_name);

	client.send_packet(set_goal, goal_name);

	cout << " Цель добавлена" << endl;
	_getch();
}

void Admin::remove_goal()
{
	string name;
	bool find_status;
	load_goals();
	do
	{
		cin.sync();
		cin.clear();
		rewind(stdin);
		find_status = false;
		cout << "Удалить цель" << endl;
		Admin::print_goals_only(source, size);
		if (size == 0)
			return;
		cout << "Выберите имя: ";
		getline(cin, name);
		for (int i = 0; i < size; ++i)
			if (source[i].goal_name == name)
			{
				find_status = true;
				break;
			}
		if (!find_status)
		{
			system("cls");
			cout << "Цель с именем \"" << name << "\" не найдена. Повторите ввод" << endl;
			_getch();
		}
	} while (!find_status);

	client.send_packet(remv_goal, name);

	cout << "Цель удалена";
	_getch();
}

void Admin::goal_info()
{
	string name;
	bool find_status;
	load_goals();
	do
	{
		find_status = false;
		Admin::print_goals_only(source, size);
		if (size == 0)
			return;
		cout << "Выберите имя: ";
		getline(cin, name);
		for (int i = 0; i < size; ++i)
			if (source[i].goal_name == name)
			{
				find_status = true;
				break;
			}
		if (!find_status)
		{
			system("cls");
			cout << "Цель с именем \"" << name << "\" не найдена. Повторите ввод" << endl;
			_getch();
		}
	} while (!find_status);

	size_t amount;
	client.send_packet(get_goal_info, name);
	client.recv_packet(get_goal_info, (char*)& amount, sizeof(size_t));
	Mark* mark = new Mark[amount];
	int size = 0;
	char buf[1000];
	for (int i = 0; i < amount; ++i)
	{
		client.recv_packet(get_goal_info, (char*) & (mark[i].mark), sizeof(double));
		client.recv_packet(get_goal_info, (char*) & (mark[i].competence), sizeof(int));
		recv(client.get_sock(), (char *)&size, sizeof(int), NULL);
		recv(client.get_sock(), buf, size, NULL);
		mark[i].user_name = buf;

	}

	system("cls");
	cout.setf(ios::left);
	cout << "   Цель: " << name << endl;
	cout << " +-----------------------+--------------+----------------------+" << endl;
	cout << " | Имя эксперта          | Оценка       | Мера компетентности  |" << endl;
	cout << " |                       | эксперта     | эксперта             |" << endl;
	cout << " +-----------------------+--------------+----------------------+" << endl;
	for (int i = 0; i < amount; ++i)
	{
		cout << " | "
			<< setw(21) << mark[i].user_name << " | "
			<< setw(12) << mark[i].mark << " | "
			<< setw(20) << mark[i].competence << " |" << endl;
		cout << " +-----------------------+--------------+----------------------+" << endl;
	}
	_getch();


}

void Admin::user_manage()
{
	Select select;
	do
	{
		system("cls");
		Admin::print_users();
		cout << "1 - Добавить пользователя" << endl;
		cout << "2 - Удалить пользователя" << endl;
		cout << "3 - Выход" << endl;

		select = select_getch(1, 3);
		system("cls");
		Admin::print_users();
		switch (select)
		{
		case 1:
			Admin::add_user();
			break;
		case 2:
			Admin::remove_user();
			break;
		default:
			break;
		}
	} while (select != 3);
}