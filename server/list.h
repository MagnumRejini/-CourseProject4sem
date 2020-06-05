#pragma once

#include <iostream>
#include <iomanip>
using namespace std;

template<class T>
struct Node
{
	T data;
	Node<T>* next;
	Node<T>* prev;
	int number;
};

//класс-контейнер Список
template<class T>
class List
{
protected:
	Node<T>* head;
	Node<T>* tail;
	int amount;
public:
	List()
	{
		head = nullptr;
		tail = nullptr;
		amount = 0;
	}
	~List()
	{
		while (head)
		{
			this->pop_head();
		}
	}
	//Длина списка
	int size()
	{
		return this->amount;
	}
	void push_head(T obj);
	void push_tail(T input_object);
	T pop_head();
	T pop_tail();
	void pop(int position);
	T& operator[](int num);
	Node<T>* begin();
	Node<T>* end();
	void output();
};
//Добавление с головы
template<class T>
void List<T>::push_head(T obj)
{
	if (head == nullptr)
	{
		head = new Node<T>;
		head->data = obj;
		head->next = nullptr;
		head->prev = nullptr;
		head->number = (amount++);
		tail = head;
	}
	else
	{
		Node<T>* node = new Node<T>;
		node->data = obj;
		node->next = head;
		node->prev = nullptr;
		head->prev = node;
		head = node;
		for (int i = 0; node; node = node->next, i++)
		{
			node->number = i;
		}
		amount++;
	}
	return;
}
//Добавление в хвост
template<class T>
void List<T>::push_tail(T input_object)
{
	if (head == nullptr)
	{
		head = new Node<T>;
		head->data = input_object;
		head->next = nullptr;
		head->prev = nullptr;
		head->number = (amount++);
		tail = head;
		return;
	}
	Node<T>* node = new Node<T>;
	node->data = input_object;
	node->next = nullptr;
	node->prev = tail;
	tail->next = node;
	tail = node;
	node->number = (amount++);
	return;
}
//Удаление от головы
template<class T>
T List<T>::pop_head()
{
	if (!(head)) return T();
	T data = head->data;
	Node<T>* node = head;
	if (head != tail)
	{
		head = head->next;
		head->prev = nullptr;
		Node<T>* tmp = head;
		for (int i = 0; tmp; tmp = tmp->next, i++)
		{
			tmp->number = i;
		}
	}
	else
	{
		head = tail = nullptr;
	}
	delete node;
	amount--;
	return data;
}
//Удаление из хвоста
template<class T>
T List<T>::pop_tail()
{
	if (!(head)) return T();
	T data = tail->data;
	Node<T>* node = tail;
	if (tail != head)
	{
		tail = tail->prev;
		tail->next = nullptr;
	}
	else
	{
		head = tail = nullptr;
	}
	delete node;
	amount--;
	return data;
}

template<class T>
void List<T>::pop(int position)
{
	Node<T>* curr = this->head;
	Node<T>* temp;
	int current_pos = 0;
	while (current_pos < position && current_pos < this->size())
	{
		curr = curr->next;
		++current_pos;
	}
	temp = curr;
	if (curr == this->head)
	{
		this->head = curr->next;
		delete curr;
		--(this->amount);
	}
	else
	{
		curr = curr->prev;
		curr->next = temp->next;
		delete temp;
		--(this->amount);
	}
}

// Доступ к объектам с помощью индексации
template<class T>
T& List<T>::operator[](int num)
{
	Node<T>* curr = head;
	if (num < 0 || num >= amount) return curr->data;
	for (int i = 0; i < num; i++)
		curr = curr->next;
	return curr->data;
}

// Функции для работы с итератором
template<class T>
Node<T>* List<T>::begin()
{
	return head;
}
template<class T>
Node<T>* List<T>::end()
{
	if (tail != nullptr) return tail->next;
	return tail;
}
// Вывод на экран содержимого списка
template<class T>
void List<T>::output()
{
	int i = 0;
	for (Node<T>* node = head; node != nullptr; node = node->next)
	{
		cout << "|" << setw(3) << i + 1 << node->data;
		cout << endl;
		i++;
	}
}
