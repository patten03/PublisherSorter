#include "Sorting.h"

void standartSettings()
{
	// установка русской кодировки для консоли
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::setlocale(LC_ALL, "ru");
}

void menu() {
	std::cout << "Добро пожаловать в программу PublisherSorter" << std::endl << std::endl
		<< "Программа позволяет обрабатывать таблицы HTML с информацией об изданиях" << std::endl
		<< "В функционал программы входит сортировка данных по следующим полям:" << std::endl

		<< "- Название издания;" << std::endl
		<< "- Вид издания;" << std::endl
		<< "- Издающая организация;" << std::endl
		<< "- Год издания;" << std::endl
		<< "- Адрес редакции;" << std::endl
		<< "- Фамилия главного редактора;" << std::endl 
		<< "Сортировка может происходить как по возрастанию, так и по убыванию" << std::endl << std::endl

		<< "Для работы с файлами они должны находится в той же директории, что и программа" << std::endl << std::endl;

	bool quit(false); // переменная выхода из программы
	while (!quit) {
		std::cout << "Выберите действие:" << std::endl;
		std::vector<std::string> menuPanel{
			"Выбор файла",
			"Выход из программы"
		};
		ask(menuPanel);
		int choice = inputChoice(menuPanel.size());

		std::string file;
		switch (choice) {
		case 1: {
			file = findFile("Выберите файл:");
			if (file != "") {
				fixFile(file);
				createSortedFile(file);
			}
			break;
		}
		case 2: {
			quit = true;
			break;
		}
		default:
			break;
		}
	}
}

void ask(const std::vector<std::string> choice) {
	for (int i(0); i < choice.size(); i++)
	{
		std::cout << i + 1 << ". " << choice[i] << std::endl;
	}
}

int inputChoice(const int& end) {
	int choiceInt = _getch();

	// цикл прерывается только при нажатии клавиши от 1 до <end>
	while (choiceInt <= '0' or choiceInt > char(end + '0'))
		choiceInt = _getch();

	choiceInt = choiceInt - '0'; // преобразование значения цифры из char в int 
	system("cls");
	return choiceInt;
}

std::string askString(const std::string& question) {
	std::string res;
	std::cout << question << std::endl;

	bool approved(false); // переменная цикла, становится true когда строка введена без ошибок
	while (!approved)
	{
		try
		{
			std::cout << ">>";
			std::getline(std::cin, res);
			checkSpecialSymbols(res); // запрет на ввод символов \/:*?<>"|
			if (res != "")
				approved = true;
			else
			{
				system("cls");
				std::cout << question << std::endl;
			}
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}
	system("cls");
	return res;
}

void createSortedFile(const std::string& file) {
	sortingSettings settings = setNewFile(file);
	std::vector<book> books = readFile(file);
	insertionSort(books, settings);

	std::fstream res;
	res.open(settings.name + ".html", std::ios_base::out);

	std::string top("<!DOCTYPE html><html>");

	std::string fieldName;
	switch (settings.field) {
	case name: fieldName = "НИЗДАНИЯ"; break;
	case kind: fieldName = "ВИД ИЗДАНИЯ"; break;
	case organization: fieldName = "ИЗДАЮЩАЯ ОРГАНИЗАЦИЯ"; break;
	case year: fieldName = "ГОД ИЗДАНИЯ"; break;
	case address: fieldName = "АДРЕС РЕДАКЦИИ"; break;
	case surname: fieldName = "ФАМИЛИЯ ГЛАВНОГО РЕДАКТОРА"; break;
	}

	std::string direction;
	switch (settings.isReversed) {
	case false: direction = "по ВОЗРАСТАНИЮ"; break;
	case true: direction ="по УБЫВАНИЮ"; break;
	}

	top = top + "<body><h2>" + "Сортировка по полю " + fieldName + " " + direction + "</h2>";
	top = top + "<style> table, th, td{ border:1px solid black; }</style><table>";

	res << top << headerRow(settings.field);

	for (auto const& curBook : books) {
		res << row(curBook, settings.field) << std::endl;
	}

	std::string floor("</table></body>");
	res << floor;

	res.close();

	std::cout << "Ваш файл под названием " << settings.name << ".html" << " был создан" << std::endl;
}

std::string row(const book & doc) {
	std::string res;
	res = res + "<tr>";
	res = res + "<td>" + doc.name + "</td>";
	res = res + "<td>" + doc.kind + "</td>";
	res = res + "<td>" + doc.organization + "</td>";
	res = res + "<td>" + doc.year + "</td>";
	res = res + "<td>" + doc.address + "</td>";
	res = res + "<td>" + doc.surname + "</td>";
	res = res + "</tr>";

	return res;
}

std::string headerRow() {
	std::string res;
	res = res + "<th>" + "Название издания" + "</th>";
	res = res + "<th>" + "Вид издания" + "</th>";
	res = res + "<th>" + "Издающая организация" + "</th>";
	res = res + "<th>" + "Год издания" + "</th>";
	res = res + "<th>" + "Адресс редакции" + "</th>";
	res = res + "<th>" + "Фамилия главного редактора" + "</th>" + "\n";
	return res;
}

std::string row(const book& doc, typeField mainField) {

	std::vector<std::string> fieldList{
		doc.name,
		doc.kind,
		doc.organization,
		doc.year,
		doc.address,
		doc.surname
	};

	std::string res;
	fieldList[0].swap(fieldList[mainField - 1]);

	res = res + "<tr>";
	for (auto line : fieldList) {
		res = res + "<td>" + line + "</td>";
	}
	res = res + "</tr>\n";

	//res = res + "<tr>";
	//res = res + "<td>" + doc.name + "</td>";
	//res = res + "<td>" + doc.kind + "</td>";
	//res = res + "<td>" + doc.organization + "</td>";
	//res = res + "<td>" + doc.year + "</td>";
	//res = res + "<td>" + doc.address + "</td>";
	//res = res + "<td>" + doc.surname + "</td>";
	//res = res + "</tr>";

	return res;
}

std::string headerRow(typeField mainField) {
	std::vector<std::string> fieldList{
		"Название издания",
		"Вид издания",
		"Издающая организация",
		"Год издания",
		"Адрес редакции",
		"Фамилия главного редактора"
	};

	std::swap(fieldList[0], fieldList[mainField - 1]);

	std::string res;
	for (auto line : fieldList) {
		res = res + "<th>" + line + "</th>";
	}
	res = res + "\n";

	return res;
}

std::string swapFields(std::vector<std::string> fieldList, typeField mainField) {
	fieldList[0].swap(fieldList[mainField - 1]);

	std::string res;
	for (auto line : fieldList) {
		res = res + "<th>" + line + "</th>";
	}
	res = res + "\n";

	return res;
}

sortingSettings setNewFile(const std::string& file) {
	sortingSettings res;

	std::cout << "Выберите поле, по которому будете сортировать таблицу:" << std::endl;
	std::vector<std::string> fields{
		"Название издания",
		"Вид издания",
		"Издающая организация",
		"Год издания",
		"Адрес редакции",
		"Фамилия главного редактора"
	};
	ask(fields);
	res.field = typeField(inputChoice(fields.size()));

	std::cout << "Выберите направление сортировки" << std::endl;
	std::vector<std::string> direction{
		"Сортировка по возрастанию",
		"Сортировка по убыванию"
	};
	ask(direction);
	res.isReversed = bool(inputChoice(direction.size()) - 1);

	res.name = askString("Введите название файла");

	return res;
}

void fixFile(const std::string& file)
{
	std::fstream raw, fixed;
	raw.open(file, std::ios_base::in);

	// Требуется ли обрабатывать файл
	std::string check;
	raw.ignore(246 - 1, '\n'); // 246 - длинна "шапки" файла
	std::getline(raw, check, '\n');

	raw.clear();
	raw.seekg(0);

	if (check != "") {
		fixed.open("temp.html", std::ios_base::out);
		while (!raw.eof()) {
			std::string buff;
			std::getline(raw, buff, '>'); // знак разделения HTML файла
			if (buff != "") {
				if (buff.find("<tr") != -1) {
					fixed << buff << '>' << std::endl;
				}
				else
					fixed << buff << '>';
			}
		}

		raw.close();
		fixed.close();

		std::string filename = file.substr(file.rfind('\\') + 1, file.length());
		system(("del \"" + filename + "\"").c_str());
		system(("ren temp.html \"" + filename + "\"").c_str());
	}
}

std::vector<book> readFile(const std::string& file)
{
	std::vector<book> res;

	std::fstream fileStream;
	fileStream.open(file, std::ios_base::in);

	fileStream.ignore(2056,'\n');

	while (!fileStream.eof()) {
		std::string buffRow;
		std::getline(fileStream, buffRow);
		book buffBook = splitRow(buffRow);
		res.push_back(buffBook);
	}

	fileStream.close();

	return res;
}

book splitRow(std::string row)
{
	book res;

	int beg, end;
	int leftEdge = std::string("<td>").length();
	int rigthEdge = std::string("</td>").length();
	beg = row.find("<td>") + leftEdge;
	end = row.find("</td>");

	std::string buff[6];

	//разделение строки по <td> и </td>
	for (auto& field : buff)
	{
		field = row.substr(leftEdge, end - beg);
		row.erase(0, end + rigthEdge);
		
		end = row.find("</td>");
	}

	res.name = buff[0];
	res.kind = buff[1];
	res.organization = buff[2];
	res.year = buff[3];
	res.address = buff[4];
	res.surname = buff[5];

	return res;
}

void insertionSort(std::vector<book> &books, sortingSettings set) {
	for (int counter = 0; counter < books.size(); counter++) {
		book currentBook = books[counter];
		int j = counter - 1;
		while (j >= 0 && naturalComparisonGreater(books[j], currentBook, set)) {
			books[j + 1] = books[j];
			j--;
		}
		books[j + 1] = currentBook;
	}
}

//@return равносильно знаку >
bool naturalComparisonGreater(book book1, book book2, sortingSettings set) {
	bool res (false);

	std::string str1, str2;

	//Передача поля, по которому идет сортировка
	switch (set.field) {
	case name: {
		str1 = book1.name;
		str2 = book2.name;
		break;
	}
	case kind: {
		str1 = book1.kind;
		str2 = book2.kind;
		break;
	}
	case organization: {
		str1 = book1.organization;
		str2 = book2.organization;
		break;
	}
	case year: {
		str1 = book1.year;
		str2 = book2.year;
		break;
	}
	case address: {
		str1 = book1.address;
		str2 = book2.address;
		break;
	}
	case surname: {
		str1 = book1.surname;
		str2 = book2.surname;
		break;
	}
	defautl: break;
	}
	
	if (str1 == "None")
		return true; // чтобы всегда данное поле было внизу
	if (str2 == "None")
		return false; // чтобы всегда данное поле было внизу

	// лексикографическое сравнение
	res = std::lexicographical_compare(
		str1.begin(), str1.end(),
		str2.begin(), str2.end());

	// настройка для сортировки в обратном порядке
	if (set.isReversed)
		return !res;
	else
		return res;
}

void checkSpecialSymbols(const std::string& word) {
	std::string forbiddenSymbols("\\/:*?<>\"|");
	for (int i(0); i < forbiddenSymbols.size(); i++)
	{
		if (word.find(forbiddenSymbols[i]) != -1)
			throw std::invalid_argument("Файл не может содержать \\/:*?<>\"|");
	}
}