
#ifndef SORTING
#define SORTING

#include <iostream>
#include <Windows.h>
#include <vector>
#include <conio.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <algorithm>
#include "DirWork.h"

// ������ ����� �� �������
enum typeField {
	name = 1,
	kind,
	organization,
	year,
	address,
	surname
};

// ��������� �������� ������
struct sortingSettings {
	typeField field;
	bool isReversed;
	std::string name;
};

// ��������� �����
struct book {
	std::string name;
	std::string kind;
	std::string organization;
	std::string year;
	std::string address;
	std::string surname;
};

void menu(); // ����� �������� ���� ��� ������������ � �����������

void ask(const std::vector<std::string> choice);    // ����� ������ �������� � �������
int inputChoice(const int& end);                    // ������������ �� ����� ������������ ��������
void checkSpecialSymbols(const std::string& word);  // �������� �������� ����� �� ���� �������

std::string askName(const std::string& question);   // ������ � ������������ �������� ����� � �������� �� ����������� �������

void createSortedFile(const std::string& file);      // �������� �� ����� ���������� ����� ������������� � ������� ����
sortingSettings setNewFile(const std::string& file); // ����� �������� ���������� �������������
void fixFile(const std::string& file);               // �������������� �������� html ������� � �������� ��� ��������� ���
std::vector<book> readFile(const std::string& file); // ��������� ���� ���������� �� ������� � vector
book splitRow(std::string row);                      // ��������� �� html ������� ������ � �������������� ������ � ������� ���������

void insertionSort(std::vector<book>& books, sortingSettings set);          // ���������� �������� �� ������ �������� ������������
bool naturalComparisonGreater(book book1, book book2, sortingSettings set); // ������������������ ��������� ��������� � ������ ��������� ������

std::string headerRow(typeField mainField);            // �������� ����� ������� html
std::string row(const book& doc, typeField mainField); // ��������� � html ������� ������

void standartSettings(); // �������� ������� ���������

#endif // !SORTING
