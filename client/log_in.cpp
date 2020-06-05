#include <iostream>
#include <conio.h>
#include "client.h"

extern Client client;

typedef unsigned char Select;

using namespace std;

extern Client client;
const unsigned short LOGIN_MIN_SIZE = 6;
const unsigned short PASSWORD_MIN_SIZE = 6;
const unsigned short LOGIN_MAX_SIZE = 30;
const unsigned short PASSWORD_Max_SIZE = 30;
const char* BANNED_SYMBOLS = " !@#$%^&*()-_=+{}[]\\|'\"<>,./?";


Select select_getch(int left, int right)
{
	Select c;
	do
	{
		c = _getch();
		if (c == 27)
			return right;
		c -= '0';

	} while (c < left || c > right);

	return c;
}

unsigned correct_str(string* str)
{
	cin >> *str;
	if (str->length() < 6)
		return 1;
	if (str->length() > 30)
		return 2;
	return 0;
}

Authorization_type log_in()
{
	Select select;
	Authorization_type authorization_mode;
	string login, password, packet;
	do
	{
		system("cls");
		cout << "������� �����: ";
		while (select = correct_str(&login))
		{
			switch (select)
			{
			case 1:
				cout << "����� ����� ��������� ��� ������� " << LOGIN_MIN_SIZE << " ��������";
				break;
			case 2:
				cout << "����� ������ ��������� �� ������ " << LOGIN_MAX_SIZE << " ��������";
			case 3:
				cout << "����� �� ������ ��������� ����������� �������� \"" << BANNED_SYMBOLS << "\"";
				break;
			default:
				break;
			}
			_getch();
			system("cls");
			cout << "������� �����: ";
		}

		system("cls");
		cout << "������� ������: ";
		while (select = correct_str(&password))
		{
			switch (select)
			{
			case 1:
				cout << "������ ����� ��������� ��� ������� " << LOGIN_MIN_SIZE << " ��������";
				break;
			case 2:
				cout << "������ ������ ��������� �� ������ " << LOGIN_MAX_SIZE << " ��������";
			case 3:
				cout << "������ �� ������ ��������� ����������� �������� \"" << BANNED_SYMBOLS << "\"";
				break;
			default:
				break;
			}
			_getch();
			system("cls");
			cout << "������� ������: ";
		}

		packet = login + "|" + password;         // ������� � ����������� ������ ����������
		client.send_packet(authorization, packet);          // �������� �������� �������
		client.send_request(authorization_status);
		client.recv_packet(authorization_status, (char*)& authorization_mode, sizeof(Authorization_type));

		if (authorization_mode == not_authorized)
		{
			cout << "������������ �� ������" << endl;
			cout << "1 - ��������� ����" << endl
				 << "2 - �����" << endl;
			if (select_getch(1, 2) == 2)
				break;
		}
	}
	while (authorization_mode == not_authorized);
	return authorization_mode;
}

void registration()
{
	string login, pass, confirm_pass;
	system("cls");
	cout << "����������� ������������" << endl << endl;
	cout << "������� �����: ";
	cin >> login;
	do
	{
		cout << "������� ������: ";
		cin >> pass;
		system("cls");
		cout << "����������� ������: ";
		cin >> confirm_pass;
		if (pass != confirm_pass)
		{
			system("cls");
			cout << "������ �� ���������";
			_getch();
		}
	} while (pass != confirm_pass);
	string packet = login + "|" + pass;
	client.send_packet(add_usr, packet);
}
