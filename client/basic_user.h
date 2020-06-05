#pragma once
#include <iostream>
#include <iomanip>
using namespace std;
typedef unsigned char Select;
Select select_getch(int left, int right);

extern Client client;

class Basic_user
{
protected:
	static User_goal* source;
	static size_t size;
	static void load_goals();
	static void print_goals_only(User_goal* buf, size_t size);
	static void print_selected(User_goal* buf, size_t size);
	static void delete_elem(User_goal* buf, size_t pos, size_t size);
	static void sort(User_goal* buf, size_t size, int mode);
	static void reverse(User_goal* buf, size_t size);
public:
	static void print_table();
	static void sort();
	static void filtr();
};

User_goal* Basic_user::source;
size_t Basic_user::size = 0;

void Basic_user::print_goals_only(User_goal* buf, size_t size)
{
	if (size == 0)
	{
		cout << "Список целей отсутствует" << endl;
		_getch();
		return;
	}
	cout << "Цели:" << endl;
	for (int i = 0; i < size - 1; ++i)
		cout << ' ' << buf[i].goal_name << ',' << endl;
	cout << ' ' << buf[size - 1].goal_name << '.' << endl;
}

void Basic_user::load_goals()
{
	double mark;
	client.send_request(get_table_size);
	client.recv_packet(get_table_size, (char*)& size, sizeof(size_t));
	source = new User_goal[size];

	client.send_request(get_table);
	for (int i = 0; i < size; ++i)
	{
		client.recv_packet(get_table, (char*)(source + i), sizeof(User_goal));
	}
}

void Basic_user::reverse(User_goal* buf, size_t size)
{
	User_goal tmp;
	for (int i = 0; i < size / 2; ++i)
	{
		tmp = buf[i];
		buf[i] = buf[size - i - 1];
		buf[size - i - 1] = tmp;
	}
}

void Basic_user::sort(User_goal* buf, size_t size, int mode)
{
	User_goal temp;
	bool swap_value;
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			switch (mode)
			{
			case 1:
				swap_value = buf[j].goal_weight < buf[j + 1].goal_weight;
				break;
			case 2:
				swap_value = buf[j].average_competence < buf[j + 1].average_competence;
				break;
			case 3:
				swap_value = buf[j].max_mark < buf[j + 1].max_mark;
				break;
			case 4:
				swap_value = buf[j].min_mark > buf[j + 1].min_mark;
				break;
			default:
				cout << "error" << endl;
				break;
			}
			if (swap_value) {
				temp = buf[j];
				buf[j] = buf[j + 1];
				buf[j + 1] = temp;
			}
		}
	}
}



void Basic_user::delete_elem(User_goal* buf, size_t pos, size_t size)
{
	for (int i = pos; i < size - 1; ++i)
		buf[i] = buf[i + 1];
}

void Basic_user::print_selected(User_goal* buf, size_t size)
{
	cout.setf(ios::left);
	cout << " +-----------------------+-----------+------------------------+---------------------+--------------------+" << endl;
	cout << " | Название цели         | Вес цели  | Средняя компетентность | Максимальная оценка | Минимальная оценка |" << endl;
	for (int i = 0; i < size; ++i)
	{
		cout << " +-----------------------+-----------+------------------------+---------------------+--------------------+" << endl;
		if (buf[i].goal_weight == -1)
		{
			cout << " | "
				<< setw(21) << buf[i].goal_name << " | "
				<< setw(9) << '-' << " | "
				<< setw(22) << '-' << " | "
				<< setw(19) << '-' << " | "
				<< setw(18) << '-' << " |" << endl;
		}
		else
		{
			cout << " | "
				<< setw(21) << buf[i].goal_name << " | "
				<< setw(9) << buf[i].goal_weight << " | "
				<< setw(22) << buf[i].average_competence << " | "
				<< setw(19) << buf[i].max_mark << " | "
				<< setw(18) << buf[i].min_mark << " |" << endl;
		}
	}
	cout << " +-----------------------+-----------+------------------------+---------------------+--------------------+" << endl;
}

void Basic_user::print_table()
{
	load_goals();

	Basic_user::print_selected(source, size);
	_getch();
}

void Basic_user::sort()
{
	Select select;
	load_goals();

	enum Direction{direct, reverse};
	Direction next_sort_direction = direct;
	Select prev_select = 1000;
	do
	{
		system("cls");
		Basic_user::print_selected(source, size);

		cout << " Выберите:" << endl;
		cout << " 1 - Сортировка по весу" << endl;
		cout << " 2 - Сортировка по средней компетентности" << endl;
		cout << " 3 - Сортировка по максимальной оценке" << endl;
		cout << " 4 - Сортировка по минимальной оценке" << endl;
		cout << " 5 - Выход" << endl;

		select = select_getch(1, 5);

		if (select == 5)
			break;

		if (prev_select == select)
		{
			Basic_user::reverse(source, size);
		}
		else
		{
			Basic_user::sort(source, size, select);
		}

		prev_select = select;
	} while (select != 5);
}

void Basic_user::filtr()
{
	Select select;
	double mark;
	load_goals();

	int modify_size = size;
	User_goal* modify = new User_goal[size];
	for (int i = 0; i < size; ++i)
		modify[i] = source[i];

	double max, min;
	do
	{
		system("cls");
		if (modify_size != 0)
		{
			Basic_user::print_selected(modify, modify_size);
			cout << " Выберите:" << endl;
			cout << " 1 - Фильтрация по весу" << endl;
			cout << " 2 - Фильтрация по средней компетентности" << endl;
			cout << " 3 - Фильтрация по максимальной оценке" << endl;
			cout << " 4 - Фильтрация по минимальной оценке" << endl;
			cout << " 5 - Сбросить параметры" << endl;
			cout << " 6 - Выход" << endl;
			select = select_getch(1, 6);
		}
		else
		{
			cout << endl << " Подходит записей: 0" << endl << endl;
			cout << " 1 - Сбросить параметры" << endl;
			cout << " 2 - Выход" << endl;
			select = select_getch(1, 2) + 4;
		}


		if (select == 5)
		{
			for (int i = 0; i < size; ++i)
				modify[i] = source[i];
			modify_size = size;
		}
		else if (select != 6)
		{
			system("cls");
			Basic_user::print_selected(modify, modify_size);
			cout << " Введите максимальное значение: ";
			cin >> max;
			cout << " Введите минимальное значение: ";
			cin >> min;
		}

		for (int i = modify_size - 1; i >= 0; --i)
		{
			switch (select)
			{
			case 1:
				if (modify[i].goal_weight > max || modify[i].goal_weight < min)
					delete_elem(modify, i, modify_size--);
				break;
			case 2:
				if (modify[i].average_competence > max || modify[i].average_competence < min)
					delete_elem(modify, i, modify_size--);
				break;
			case 3:
				if (modify[i].max_mark > max || modify[i].max_mark < min)
					delete_elem(modify, i, modify_size--);
				break;
			case 4:
				if (modify[i].min_mark > max || modify[i].min_mark < min)
					delete_elem(modify, i, modify_size--);
				break;
			case 5:
				
				break;
			case 6:
				break;
			default:
				break;
			}
		}

	} while (select != 6);
}