#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <string>
#include "client.h"
#include "interface.h"
#include "admin.h"
#include "user.h"

const char* SERVER_ADDR = "127.0.0.1";
Client client;

Authorization_type log_in();
void registration();

int main()
{
	Select select_getch(int left, int right);

	setlocale(LC_ALL, "Russian");
	client.init_library();
	client.set_addr(SERVER_ADDR, 1111, ip_v4);
	client.connect_to_server();
	
	Select select;
	string login, password;
	string packet;
	Authorization_type authorization_mode;

	do
	{
		authorization_mode = not_authorized;
		select = Interface::extern_menu();

		switch (select)
		{
		case 1:
			authorization_mode = log_in();
			break;
		case 2:
			registration();
			break;
		case 3:
			client.send_request(close_connection);
			break;
		default:
			break;
		}

		int post;
		if (select != 3)
		{
			client.send_request(get_post);
			client.recv_packet(get_post, (char*)& post, sizeof(int));
		}
		switch (authorization_mode)
		{
		case user:
			if (post == -1)
			{
				system("cls");
				cout << "Необходимо заполнить данные" << endl << "нажмите любую клавишу..." << endl;
				_getch();
				system("cls");
				User::edit_info();
				_getch();
			}

			Interface::user_menu();
			break;
		case admin:
			Interface::admin_menu();
			break;
		default:
			break;
		}

	} while (select != 3);

	system("cls");
	cout << "Данные сохранены" << endl;
	_getch();
}

