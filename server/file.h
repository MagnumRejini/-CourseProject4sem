#include <fstream>
#include "List.h"
#include "log_in.h"
#include "data.h"
#define KEY "h438938vj398v";

template <class T>
class File
{
public:
	File() {};
	~File() {};

	void open_goals(List <T>& list, const char* file);
	void OpenLogPass(List <T>& list, const char* file);

	void save_goals(List<T>& list, const char* file);
	void SaveUsers(List<T>& list, const char* file);
};

template <class T>
void File<T>::OpenLogPass(List <T>& list, const char* file)
{
	ifstream in;
	in.open(file);
	string a;
	if (!in.is_open())
	{
		cout << "Невозможно открыть файл!" << endl;
		return;
	}
	while (true)
	{
		string login;
		string pass;
		int post;
		int degree;
		string key = KEY;
		in >> login >> pass >> post >> degree;
		if (login == "%") break;
		for (int j = 0; j < login.size(); ++j)
		{
			login[j] -= key[j % key.size()];
		}
		for (int j = 0; j < pass.size(); ++j)
		{
			pass[j] -= key[j % key.size()];
		}
		User tmp(login, pass, post, degree);
		list.push_tail(tmp);
	}
	in.close();
}

template <class T>
void File<T>::open_goals(List <T>& list, const char* file)
{
	ifstream in;
	in.open(file, ios::in);
	if (!in.is_open())
	{
		cout << "Невозможно открыть файл!" << endl;
		return;
	}
	string name;
	string user_name;
	double mark;
	int competence;
	List<Mark> m_list;
	int m_pos;

	in >> name;
	Goal g_tmp;
	while (true)
	{
		in >> name;
		if (name == "%")
			break;
		for (int j = 0; j < name.size(); ++j)
			if (name[j] == '_')
				name[j] = ' ';
		g_tmp.name = name;
		m_pos = 0;
		while (true)
		{
			in >> user_name;
			if (user_name == "|")
				break;
			in >> mark >> competence;

			g_tmp.mark[m_pos].user_name = user_name;
			g_tmp.mark[m_pos].mark = mark;
			g_tmp.mark[m_pos].competence = competence;
			++m_pos;
		}
		g_tmp.amount = m_pos;
		list.push_tail(g_tmp);
	}
	in.close();
}

template <class T>
void File<T>::save_goals(List <T>& list, const char* file)
{
	ofstream on;
	on.open(file, ios::in | ios_base::trunc);
	if (!on.is_open())
	{
		cout << "Невозможно открыть файл!" << endl;
		return;
	}
	on << "c ";
	for (int i = 0; i < list.size(); ++i)
	{
		for (int j = 0; j < list[i].name.size(); ++j)
			if (list[i].name[j] == ' ')
				list[i].name[j] = '_';
		on << list[i].name << " ";
		for (int j = 0; j < list[i].amount; ++j)
		{
			on << list[i].mark[j].user_name << " " << list[i].mark[j].mark << " " << list[i].mark[j].competence << " ";
		}
		on << "| " << endl;
	}
	on << "%";
	on.close();
}

template <class T>
void File<T>::SaveUsers(List<T>& list, const char* file)
{
	string key = KEY;
	ofstream on;
	on.open(file, ios::in | ios_base::trunc);
	if (!on.is_open())
	{
		cout << "Невозможно открыть файл!" << endl;
		return;
	}
	for (int i = 0; i < list.size(); i++)
	{
		for (int j = 0; j < list[i].login.size(); ++j)
		{
			list[i].login[j] += key[j % key.size()];
		}
		for (int j = 0; j < list[i].pass.size(); ++j)
		{
			list[i].pass[j] += key[j % key.size()];
		}
		on << list[i].login << " " << list[i].pass << " " << list[i].post << " " << list[i].degree << " " << endl;
	}
	on << "%";
	on.close();
}

