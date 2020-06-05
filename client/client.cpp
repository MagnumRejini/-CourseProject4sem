#include "client.h"
#include <iostream>
#include <string>
using namespace std;

const char* post[7] = { "Ведущий инженер", "С.Н.С., Н.С., М.Н.С.", "Гл.Н.С., вед.Н.С.",  "Зав.лабораторией, сектора", "Зав.отдела, заместитель", "Руководитель комплекса, отделения", "Директор, заместитель"};
const char* degree[4] = { "специалист без степени", "кандидат наук", "доктор наук", "академик" };

const unsigned short competence_matrix[7][4] =
{
	{1, 0, 0, 0},
	{1, 2, 0, 0},
	{0, 3, 4, 0},
	{2, 3, 5, 6},
	{2, 4, 6, 7},
	{2, 5, 6, 8},
	{4, 6, 8, 10}
};

void Client::send_packet(packet_type type, string data)
{
	send(sock, (char*)& type, sizeof(packet_type), NULL);
	int data_size = data.length() + 1;
	send(sock, (char*)& data_size, sizeof(int), NULL);
	send(sock, data.c_str(), data_size, NULL);
}

void Client::send_packet(packet_type type, char* data, size_t size)
{
	send(sock, (char*)& type, sizeof(packet_type), NULL);
	send(sock, (char*)& size, sizeof(size_t), NULL);
	send(sock, data, size, NULL);
}



void Client::recv_packet(const packet_type type, string data)
{
	packet_type recv_type;
	recv(sock, (char*)& recv_type, sizeof(packet_type), NULL);

	if (recv_type != type)
	{
		cout << "discrepancy of types" << endl;
		exit(3);
	}

	unsigned data_size;
	recv(sock, (char*)& data_size, sizeof(int), NULL);

	char* char_data = new char[data_size];
	recv(sock, char_data, data_size, NULL);
	data = char_data;
}

void Client::recv_packet(const packet_type type, char* data, size_t size)
{
	packet_type recv_type;
	recv(sock, (char*)& recv_type, sizeof(packet_type), NULL);

	if (recv_type != type)
	{
		cout << "discrepancy of types" << endl;
		exit(3);
	}

	unsigned data_size;
	recv(sock, (char*)& data_size, sizeof(int), NULL);

	recv(sock, data, data_size, NULL);
}