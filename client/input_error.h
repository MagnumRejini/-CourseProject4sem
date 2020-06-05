#pragma once

#include "my_exception.h"
#include <iostream>
#include <string>
namespace Error {
	class InputError : public MyException {
	public:
		InputError(int _code = 0) : MyException(_code) {};

		void ErrorText() final
		{
			cout << "������ ����� (��� " << code << "): ";
			switch (code)
			{
			case 1:
			{
				cout << "����� ������������ ������." << endl;
				cout << "���� ����� ��������, ����� ����, ��������." << endl << endl;
				break;
			}
			case 2:
			{
				cout << "������� ������������ ���������� ��������" << endl << endl;
				break;                                                              
			}
			case 3:
			{
				cout << "�� ����� ������� ������ ����� ���� �������� ������ �� ����� �����!" << endl << endl;
				break;
			}
			default:
				cout << "��������� ����" << endl;
				break;
			}
		}
		static double Input(double min, double max)
		{
			bool flag = true;
			double value;
			do
			{
				try
				{
					cin.sync();
					cin.clear();
					rewind(stdin);
					cin >> value;
					flag = true;
					if (!cin || cin.peek() != '\n')
					{
						flag = false;
						throw InputError(3);
					}
					if (value < min || value > max)
					{
						flag = false;
						throw "Limit error";
					}
				}
				catch (InputError e)
				{
					e.ErrorText();
					cin.sync();
					cin.clear();
					rewind(stdin);
				}
				catch (...)
				{
					cin.sync();
					cin.clear();
					rewind(stdin);
					cout << "������� ������������ ��������. ���������� ������ �������� � ��������� (" << min << "," << max << ")." << endl;
				}
			} while (!flag);
			return value;
		}


		static void RightNum(string input) throw (InputError)
		{
			cin.clear();
			rewind(stdin);
			int len = input.length();
			int i = 0;
			while (i < len)
			{
				if (input[i] < '0' || input[i] > '9')
				{
					throw InputError(1);
				}
				i++;
			}
		}

		static string InputNum()
		{
			string input;
			try
			{
				rewind(stdin);
				cin.sync();
				cin.clear();
				cin >> input;
				RightNum(input);
			}
			catch (InputError e)
			{
				e.ErrorText();
				cin.clear();
				cin.sync();
				input = InputNum();
			}
			rewind(stdin);
			return input;
		}
	};
}
