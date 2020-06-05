#pragma once
#include "user.h"
#include "admin.h"

typedef unsigned char Select;
Select select_getch(int left, int right);

class Interface
{
public:
	static Select extern_menu();
	static void admin_menu();
	static void user_menu();
};

Select Interface::extern_menu()
{
	system("cls");
	cout << "1 - ����" << endl;
	cout << "2 - �����������" << endl;
	cout << "3 - �����" << endl;
	return select_getch(1, 3);
}

void Interface::admin_menu()
{
	Select select;
	do
	{
		system("cls");
		cout << " 1 - �������� ����" << endl;
		cout << " 2 - ������� ����" << endl;
		cout << " 3 - ���������� ������� �����" << endl;
		cout << " 4 - ����������" << endl;
		cout << " 5 - ����������" << endl;
		cout << " 6 - ���������� � ����" << endl;
		cout << " 7 - ���������� ��������������" << endl;
		cout << " 8 - �����" << endl;

		select = select_getch(1, 8);
		system("cls");
		switch (select)
		{
		case 1:
			Admin::add_goal();
			break;
		case 2:
			Admin::remove_goal();
			break;
		case 3:
			Admin::print_table();
			break;
		case 4:
			Admin::sort();
			break;
		case 5:
			Admin::filtr();
			break;
		case 6:
			Admin::goal_info();
			break;
		case 7:
			Admin::user_manage();
			break;
		default:
			break;
		}
	} while (select != 8);
}

void Interface::user_menu()
{

	Select select;
	do
	{
		system("cls");
		cout << " 1 - �������� ������ ����" << endl;
		cout << " 2 - ���������� ������� �����" << endl;
		cout << " 3 - ����������" << endl;
		cout << " 4 - ����������" << endl;
		cout << " 5 - ������������� ���������� � ����" << endl;
		cout << " 6 - �����" << endl;

		select = select_getch(1, 6);
		system("cls");
		switch (select)
		{
		case 1:
			User::add_mark();
			break;
		case 2:
			User::print_table();
			break;
		case 3:
			User::sort();
			break;
		case 4:
			User::filtr();
			break;
		case 5:
			User::edit_info();
			break;
		case 6:
			break;
		default:
			break;
		}
	} while (select != 6);
}

