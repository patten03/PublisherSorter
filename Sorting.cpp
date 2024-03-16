#include "Sorting.h"

void standartSettings()
{
	// ��������� ������� ��������� ��� �������
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::setlocale(LC_ALL, "ru");
}

void menu() {
	std::cout << "����� ���������� � ��������� PublisherSorter" << std::endl << std::endl
		<< "��������� ��������� ������������ ������� HTML � ����������� �� ��������" << std::endl
		<< "� ���������� ��������� ������ ���������� ������ �� ��������� �����:" << std::endl

		<< "- �������� �������;" << std::endl
		<< "- ��� �������;" << std::endl
		<< "- �������� �����������;" << std::endl
		<< "- ��� �������;" << std::endl
		<< "- ����� ��������;" << std::endl
		<< "- ������� �������� ���������;" << std::endl 
		<< "���������� ����� ����������� ��� �� �����������, ��� � �� ��������" << std::endl << std::endl

		<< "��� ������ � ������� ��� ������ ��������� � ��� �� ����������, ��� � ���������" << std::endl << std::endl;

	bool quit(false); // ���������� ������ �� ���������
	while (!quit) {
		std::cout << "�������� ��������:" << std::endl;
		std::vector<std::string> menuPanel{
			"����� �����",
			"����� �� ���������"
		};
		ask(menuPanel);
		int choice = inputChoice(menuPanel.size());

		std::string file;
		switch (choice) {
		case 1: {
			file = findFile("�������� ����:");
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

void ask(const std::vector<std::string> choice)
{
	for (int i(0); i < choice.size(); i++)
	{
		std::cout << i + 1 << ". " << choice[i] << std::endl;
	}
}

int inputChoice(const int& end)
{
	int choiceInt = _getch();

	// ���� ����������� ������ ��� ������� ������� �� 1 �� <end>
	while (choiceInt <= '0' or choiceInt > char(end + '0'))
		choiceInt = _getch();

	choiceInt = choiceInt - '0'; // �������������� �������� ����� �� char � int 
	system("cls");
	return choiceInt;
}

std::string askString(const std::string& question)
{
	std::string res;
	std::cout << question << std::endl;

	bool approved(false); // ���������� �����, ���������� true ����� ������ ������� ��� ������
	while (!approved)
	{
		try
		{
			std::cout << ">>";
			std::getline(std::cin, res);
			checkSpecialSymbols(res); // ������ �� ���� �������� \/:*?<>"|
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

void createSortedFile(const std::string& file)
{
	sortingSettings settings = setNewFile(file);
	std::vector<book> books = readFile(file);
	insertionSort(books, settings);

}

sortingSettings setNewFile(const std::string& file)
{
	sortingSettings res;

	std::cout << "�������� ����, �� �������� ������ ����������� �������:" << std::endl;
	std::vector<std::string> fields{
		"�������� �������",
		"��� �������",
		"�������� �����������",
		"��� �������",
		"����� ��������",
		"������� �������� ���������"
	};
	ask(fields);
	res.field = typeField(inputChoice(fields.size()));

	std::cout << "�������� ����������� ����������" << std::endl;
	std::vector<std::string> direction{
		"���������� �� �����������",
		"���������� �� ��������"
	};
	ask(direction);
	res.isReversed = bool(inputChoice(direction.size()) - 1);

	res.name = askString("������� �������� �����");

	return res;
}

void fixFile(const std::string& file)
{
	std::fstream raw, fixed;
	raw.open(file, std::ios_base::in);

	// ��������� �� ������������ ����
	std::string check;
	raw.ignore(246 - 1, '\n'); // 246 - ������ "�����" �����
	std::getline(raw, check, '\n');

	raw.clear();
	raw.seekg(0);

	if (check != "") {
		fixed.open("temp.html", std::ios_base::out);
		while (!raw.eof()) {
			std::string buff;
			std::getline(raw, buff, '>'); // ���� ���������� HTML �����
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

	//���������� ������ �� <td> � </td>
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

void insertionSort(std::vector<book> books, sortingSettings set) {
	//for (int counter = 0; counter < books.size(); counter++) {
	//	book currentBook = books[counter];
	//	int j = counter - 1;
	//	while (j >= 0 && books[j] > currentBook) {
	//		books[j + 1] = books[j];
	//		j--;
	//	}
	//	books[j + 1] = currentBook;
	//}
}

//@return ����������� ����� >
bool naturalComparisonGreater(std::string str1, std::string str2) { 
	bool res (false);

	for (int i(0); i < minimum(str1.size(), str2.size()) + 1; i++) {
		if (str1[i] != str2[i]) {
			res = str1[i] > str2[i];
			break;
		}
	}
	
	return res;
}

int minimum(int num1, int num2) {
	if (num1 > num2)
		return num2;
	else
		return num1;
}

void checkSpecialSymbols(const std::string& word) {
	std::string forbiddenSymbols("\\/:*?<>\"|");
	for (int i(0); i < forbiddenSymbols.size(); i++)
	{
		if (word.find(forbiddenSymbols[i]) != -1)
			throw std::invalid_argument("���� �� ����� ��������� \\/:*?<>\"|");
	}
}