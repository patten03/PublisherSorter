
#ifndef SORTING
#define SORTING

#include <iostream>
#include <Windows.h>
#include <vector>
#include <conio.h>
#include <filesystem>
#include <string>
#include <fstream>
#include "DirWork.h"

enum typeField {
	name = 1,
	kind,
	organization,
	year,
	address,
	surname
};

struct sortingSettings {
	typeField field;
	bool isReversed;
	std::string name;
};

struct book {
	std::string name;
	std::string kind;
	std::string organization;
	std::string year;
	std::string address;
	std::string surname;
};

void menu();

void ask(const std::vector<std::string> choice);
int inputChoice(const int& end);
void checkSpecialSymbols(const std::string& word);
std::string askString(const std::string& question);
void createSortedFile(const std::string& file);
sortingSettings setNewFile(const std::string& file);
void fixFile(const std::string& file);
std::vector<book> readFile(const std::string& file);
book splitRow(std::string row);
void insertionSort(std::vector<book> books, sortingSettings set);
bool naturalComparisonGreater(std::string str1, std::string str2);

int minimum(int num1, int num2);

void standartSettings();

#endif // !SORTING
