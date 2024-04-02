
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

// список полей из таблицы
enum typeField {
	name = 1,
	kind,
	organization,
	year,
	address,
	surname
};

// настройки создания файлов
struct sortingSettings {
	typeField field;
	bool isReversed;
	std::string name;
};

// структура полей
struct book {
	std::string name;
	std::string kind;
	std::string organization;
	std::string year;
	std::string address;
	std::string surname;
};

void menu(); // вывод главного меню для пользователя с вступлением

void ask(const std::vector<std::string> choice);    // вывод списка действий в консоль
int inputChoice(const int& end);                    // реагирование на выбор пользователя действий
void checkSpecialSymbols(const std::string& word);  // проверка названия файла на спец символы

std::string askName(const std::string& question);   // запрос у пользователя названия файла и проверка на нелегальные символы

void createSortedFile(const std::string& file);      // отвечает за выбор параметров файла пользователем и создает файл
sortingSettings setNewFile(const std::string& file); // выбор настроек сортировки пользователем
void fixFile(const std::string& file);               // преобразование исходной html таблицы в читаймый для программы вид
std::vector<book> readFile(const std::string& file); // вынесение всей информации из таблицы в vector
book splitRow(std::string row);                      // выделение из html таблицы строки и преобразование данных в удобную структуру

void insertionSort(std::vector<book>& books, sortingSettings set);          // сортировка вставкой на основе настроек пользователя
bool naturalComparisonGreater(book book1, book book2, sortingSettings set); // лексикографическое сравнение элементов с учетом отсутсвия данных

std::string headerRow(typeField mainField);            // название полей таблицы html
std::string row(const book& doc, typeField mainField); // выводимая в html таблицу строка

void standartSettings(); // устанока русской кодировки

#endif // !SORTING
