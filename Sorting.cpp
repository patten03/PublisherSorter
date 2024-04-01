#include "Sorting.h"

//@brief ��������� �������
void standartSettings() {
	// ��������� ������� ��������� ��� �������
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::setlocale(LC_ALL, "ru");
}

//@brief ����� �������� ���� � ����������� � �����������
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
		ask(menuPanel); // ����� �������� � ������� ��� �� ������ �������������
		int choice = inputChoice(menuPanel.size());

		std::string file;
		// ������� �� ����� �������� �������������
		switch (choice) {
		case 1:
		{ // ���� � ���� ������ �����
			file = findFile("�������� ����:");
			if (file != "") {
				fixFile(file);
				createSortedFile(file);
			}
			break;
		}
		case 2:
		{
			quit = true;
			break;
		}
		default:
			break;
		}
	}
}

//@brief ����� ������ �������� � �������
void ask(const std::vector<std::string> choice) {
	for (int i(0); i < choice.size(); i++) {
		std::cout << i + 1 << ". " << choice[i] << std::endl;
	}
}

//@brief ������������ �� ����� ������������ ��������
int inputChoice(const int& end) {
	int choiceInt = _getch();

	// ���� ����������� ������ ��� ������� ������� �� 1 �� <end>
	while (choiceInt <= '0' or choiceInt > char(end + '0'))
		choiceInt = _getch();

	choiceInt = choiceInt - '0'; // �������������� �������� ����� �� char � int 
	system("cls");
	return choiceInt;
}

//@return ������� ���������� ������������� �������� �����, ������������ ���� ����������� ��������
std::string askString(const std::string& question) {
	std::string res;
	std::cout << question << std::endl;

	bool approved(false); // ���������� �����, ���������� true ����� ������ ������� ��� ������
	while (!approved) {
		try {
			std::cout << ">>";
			std::getline(std::cin, res);
			checkSpecialSymbols(res); // ������ �� ���� �������� \/:*?<>"|
			if (res != "")
				approved = true;
			else {
				system("cls");
				std::cout << question << std::endl;
			}
		}
		catch (std::exception& ex) {
			std::cout << ex.what() << std::endl; // ����� ������ � �������
		}
	}
	system("cls");
	return res;
}

//@return ���������� �������� ����� ������ � ����� � ������� ��� �������������� ����������
std::string askName() {
	std::string filename = askString("������� �������� �����");

	filename = space2underscore(filename);
	filename = filename + "_" + currentTime();

	std::string fullPath = filename + ".html";

	system("cls");
	std::cout << "���� ��� ��������� " << filename << ".html ������!" << std::endl;
	return fullPath;
}

void createSortedFile(const std::string& file) {
	sortingSettings settings = setNewFile(file); // ����� �������� ���������� ��� �����
	std::vector<book> books = readFile(file);    // ���������� ������ �� �������� �����
	insertionSort(books, settings);              // ���������� ���������� ������ �� ������ ��������

	std::fstream res;
	res.open(settings.name + ".html", std::ios_base::out);

	std::string top("<!DOCTYPE html><html>");

	std::string fieldName; // �������� ����, �� �������� ������������ ����
	switch (settings.field) {
	case name: fieldName = "�������� �������"; break;
	case kind: fieldName = "��� �������"; break;
	case organization: fieldName = "�������� �����������"; break;
	case year: fieldName = "��� �������"; break;
	case address: fieldName = "����� ��������"; break;
	case surname: fieldName = "������� �������� ���������"; break;
	}

	std::string direction; // �����������, �� �������� ��� ������������ ����
	switch (settings.isReversed) {
	case false: direction = "�� �����������"; break;
	case true: direction = "�� ��������"; break;
	}

	// �������� ��������� ��� �����, ��� ������� ��������� ����������
	top = top + "<body><h2>" + "���������� �� ���� " + fieldName + " " + direction + "</h2>";
	top = top + "<style> table, th, td{ border:1px solid black; }</style><table>";

	res << top << headerRow(settings.field);

	for (auto const& curBook : books) { // ����� ��������������� ������ � ����
		res << row(curBook, settings.field) << std::endl;
	}

	std::string floor("</table></body>");
	res << floor;

	res.close();

	std::cout << "��� ���� ��� ��������� " << settings.name << ".html" << " ��� ������" << std::endl;
}

//@return ����� ������� ������ � ���� ������ � html
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

	return res;
}

//@return ����� �������� ����� � ���� ������ � html
std::string headerRow(typeField mainField) {
	std::vector<std::string> fieldList{
		"�������� �������",
		"��� �������",
		"�������� �����������",
		"��� �������",
		"����� ��������",
		"������� �������� ���������"
	};

	std::swap(fieldList[0], fieldList[mainField - 1]);

	std::string res;
	for (auto line : fieldList) {
		res = res + "<th>" + line + "</th>";
	}
	res = res + "\n";

	return res;
}

//@brief ������������ �������� ����� ��� ��������� ������������ ������� � ������� ����� ���������
std::string swapFields(std::vector<std::string> fieldList, typeField mainField) {
	fieldList[0].swap(fieldList[mainField - 1]); // ������������ ������� ���� � ��������� �����

	std::string res;
	for (auto line : fieldList) {
		res = res + "<th>" + line + "</th>";
	}
	res = res + "\n";

	return res;
}

/*@return ����� �������� �������������, ����� ���:
  ����, �� �������� ���������� ����������;
  ����� ����������� ����������;
  �������� �����;*/
sortingSettings setNewFile(const std::string& file) {
	sortingSettings res;

	// ����� ������������ ����
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

	// ����� ����������� ����������
	std::cout << "�������� ����������� ����������" << std::endl;
	std::vector<std::string> direction{
		"���������� �� �����������",
		"���������� �� ��������"
	};
	ask(direction);
	res.isReversed = bool(inputChoice(direction.size()) - 1);

	// ����� �������� �����
	res.name = askString("������� �������� �����");
	res.name = res.name + "_" + currentTime() + "_{s}";
	res.name = space2underscore(res.name);

	return res;
}

void fixFile(const std::string& file) {
	std::fstream raw, fixed;
	raw.open(file, std::ios_base::in);

	// ��������� �� ������������ ����
	std::string check;
	raw.ignore(245, '\n'); // 246 - ������ "�����" �����
	std::getline(raw, check, '\n');

	raw.clear();
	raw.seekg(0);

	if (check != "") { // ���� check != "", ������ ���� ������� �� ����� ������ � ��� ���� ��������������
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

		// ������ ������������ ����� �� ������������
		std::string filename = file.substr(file.rfind('\\') + 1, file.length());
		system(("del \"" + filename + "\"").c_str());
		system(("ren temp.html \"" + filename + "\"").c_str());
	}
}

//@return ������ ���� ������ � ������ ��� ����������� ����������
std::vector<book> readFile(const std::string& file) {
	std::vector<book> res;

	std::fstream fileStream;
	fileStream.open(file, std::ios_base::in);

	fileStream.ignore(2056, '\n'); // ������������� ������ ������ �����, ��� ���������� ������ 

	while (!fileStream.eof()) { // ������ ������ �����
		std::string buffRow;
		std::getline(fileStream, buffRow);
		book buffBook = splitRow(buffRow);
		res.push_back(buffBook);
	}

	fileStream.close();

	return res;
}

//@return ��������� html ������ �� ���� ��� ��������� book
book splitRow(std::string row) {
	book res;

	// �������, �� ������� ���������� ���� �� ������
	int beg, end;
	int leftEdge = std::string("<td>").length();
	int rigthEdge = std::string("</td>").length();
	beg = row.find("<td>") + leftEdge;
	end = row.find("</td>");

	std::string buff[6];

	//���������� ������ �� <td> � </td>
	for (auto& field : buff) {
		field = row.substr(leftEdge, end - beg);
		row.erase(0, end + rigthEdge);

		end = row.find("</td>");
	}

	// �������� ��������������� ������� ���������
	res.name = buff[0];
	res.kind = buff[1];
	res.organization = buff[2];
	res.year = buff[3];
	res.address = buff[4];
	res.surname = buff[5];

	return res;
}

//@brief ���������� ������ ������� ������� 
//@return ��������������� �������, �������� ���� ����������
void insertionSort(std::vector<book>& books, sortingSettings set) {
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

//@return ����������� ����� >
bool naturalComparisonGreater(book book1, book book2, sortingSettings set) {
	bool res(false);

	std::string str1, str2;

	//�������� ����, �� �������� ���� ����������
	switch (set.field) {
	case name:
	{
		str1 = book1.name;
		str2 = book2.name;
		break;
	}
	case kind:
	{
		str1 = book1.kind;
		str2 = book2.kind;
		break;
	}
	case organization:
	{
		str1 = book1.organization;
		str2 = book2.organization;
		break;
	}
	case year:
	{
		str1 = book1.year;
		str2 = book2.year;
		break;
	}
	case address:
	{
		str1 = book1.address;
		str2 = book2.address;
		break;
	}
	case surname:
	{
		str1 = book1.surname;
		str2 = book2.surname;
		break;
	}
defautl: break;
	}
	std::string empty = "None";
	if (str1 == empty)
		return true; // ����� ������ ������ ���� ���� �����
	if (str2 == empty)
		return false; // ����� ������ ������ ���� ���� �����

	// ������������������ ���������
	res = std::lexicographical_compare(
		str1.begin(), str1.end(),
		str2.begin(), str2.end());

	// ��������� ��� ���������� � �������� �������
	if (set.isReversed)
		return res;
	else
		return !res;
}

//@brief �������� ��������� �������� ����� �� ����������� ������� 
void checkSpecialSymbols(const std::string& word) {
	std::string forbiddenSymbols("\\/:*?<>\"|");
	for (int i(0); i < forbiddenSymbols.size(); i++) {
		if (word.find(forbiddenSymbols[i]) != -1)
			throw std::invalid_argument("���� �� ����� ��������� \\/:*?<>\"|");
	}
}