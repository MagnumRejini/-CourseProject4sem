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
			cout << "Ошибка ввода (код " << code << "): ";
			switch (code)
			{
			case 1:
			{
				cout << "Введён недопустимый символ." << endl;
				cout << "Ввод любых символов, кроме цифр, запрещён." << endl << endl;
				break;
			}
			case 2:
			{
				cout << "Введено недопустимое количество символов" << endl << endl;
				break;                                                              
			}
			case 3:
			{
				cout << "Вы ввели символы вместо числа либо превышен предел по вводу чисел!" << endl << endl;
				break;
			}
			default:
				cout << "Повторите ввод" << endl;
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
					cout << "Введено некорректное значение. Необходимо ввести значение в диапазоне (" << min << "," << max << ")." << endl;
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
