#pragma once
#include <iostream>
#include "basic_user.h"
#include "client.h"
#include "input_error.h"
using namespace std;
typedef unsigned char Select;
Select select_getch(int left, int right);

extern Client client;

class User : public Basic_user
{
public:
	static void add_mark();
	static void edit_info();
};

void User::add_mark()
{
	int size;
	double mark;
	Select select;
	client.send_request(get_table_size);
	client.recv_packet(get_table_size, (char*)& size, sizeof(int));
	User_goal* buf = new User_goal[size];

	client.send_request(get_table);
	
	system("cls");
	cout << "Списов целей: " << endl;
	for (int i = 0; i < size; ++i)
	{
		client.recv_packet(get_table, (char*)(buf + i), sizeof(User_goal));
		cout << i + 1 << " - " << buf[i].goal_name << endl;
	}
	select = select_getch(1, size) - 1;

	system("cls");
	cout << "Выбранная цель - " << buf[select].goal_name << endl;
	cout << "Введите вашу оценку (0.0 - 1.0): ";
	mark = Error::InputError::Input(0, 1);

	client.send_packet(set_mark, buf[select].goal_name, 100);
	client.send_packet(set_mark, (char *)&mark, sizeof(mark));
}

void User::edit_info()
{
	Select select;
	int p_pos, d_pos;
	do
	{
		system("cls");
		cout << "Выберите должность: " << endl;
		for (int i = 0; i < 7; ++i)
			cout << i + 1 << " - " << post[i] << endl;
		p_pos = select_getch(1, 7) - 1;

		int j = 0;
		int match_table[4];
		system("cls");
		cout << "Выберите Вашу учёную степень: " << endl;
		for (int i = 0; i < 4; ++i)
			if (competence_matrix[p_pos][i] != 0)
			{
				match_table[j] = i;
				cout << ++j << " - " << degree[i] << endl;
			}
		d_pos = match_table[select_getch(1, j) - 1];
		system("cls");

		cout << "Ваша должность - " << post[p_pos] << endl;
		cout << "Ваша учёная степень - " << degree[d_pos] << endl;
		cout << "1 - Сохранить" << endl
			<< "2 - Ввести заново" << endl;
		select = select_getch(1, 2);
	} while (select != 1);

	client.send_packet(set_post, (char*)&p_pos, sizeof(int));
	client.send_packet(set_degree, (char*)&d_pos, sizeof(int));

	cout << endl << "Данные обновлены" << endl;
	_getch();
}