#include "DirWork.h"

//@brief вывод списка файлов в консоль для их выбора пользователем 
void showDir(const std::vector<std::string>& dir) {
	for (const auto& item : dir) {
		std::cout << "  " << item << std::endl;
	}
}

//@brief выбор файла через консоль пользователем
//@return возврат выбранного файла в виде строки
std::string findFile(std::string title) {
	std::filesystem::path p = "."; // получение пути, где находится программа
	std::string curFilepath = std::filesystem::absolute(p).string();

	// выход из цикла происходит когда выбран файл или пользователь решил выйти в меню
	while (curFilepath.find(".html") == -1 and curFilepath != "") {
		try {
			std::vector<std::string> fileList;
			makeFilesList(curFilepath, fileList);
			if (fileList.size() == 0)
				throw std::invalid_argument("директория пуста, ни одного файла не найдено");
			std::cout << title << std::endl;

			std::vector<std::string> menu{
				"Для выбора файла используйте ENTER и стрелочки ВВЕРХ ВНИЗ",
				"Для выхода из меню нажмите ESC"
			};

			int begCoord = menu.size() + 1;
			int cur(begCoord); // текущая координата стрелки выбора папки

			for (const auto& line : menu) {
				std::cout << line << std::endl;
			}
			showDir(fileList);

			fileChoice(begCoord, fileList, cur, curFilepath);

			system("cls");
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			return ""; // выход в меню по ошибке
		}
	}
	return curFilepath;
}

//@brife реализация меню выбора для findFile()
void fileChoice(int begCoord, std::vector<std::string> files, int& cur, std::string& filepath) {
	movingArrow(begCoord, begCoord + files.size(), cur, 0); // показ стрелки выбора папки  

	bool chosenMenu(false);
	while (!chosenMenu) {
		switch (int c = _getch()) {
		case 224:
		{ // код нажатия на стрелки на клавиатуре 
			cur = movingArrow(begCoord, begCoord + files.size() - 1, cur, _getch());
			break;
		}
		case 27:
		{ // выход без выбора файла 
			chosenMenu = true;
			cur = begCoord;
			filepath = "";
			break;
		}
		case 13:
		{ // выбор папки 
			filepath = filepath + "\\" + files[cur - begCoord];
			chosenMenu = true;
			cur = begCoord;
			break;
		}
		default: break;
		}
	}
}

//@brief проверка на то, является ли файл отсортированным через название файла
bool isSorted(const std::string& filename) {
	bool res(false);
	// нахождение метки в названии файла
	if (filename.find("{s}") != -1)
		res = true;
	return res;
}

//@brief создание списка файлов из директории, лежащей в программе
void makeFilesList(std::string filepath, std::vector<std::string>& folderList) {
	for (auto const& dirFolder : std::filesystem::directory_iterator(filepath + "\\")) {
		{ // цикл сохраняет файлы html без меток {s}
			if ((dirFolder.is_regular_file()
				and dirFolder.path().extension() == ".html")
				and !isSorted(dirFolder.path().string())) {

				std::string path = dirFolder.path().string();
				path = path.substr(path.rfind("\\") + 1, path.size());

				folderList.push_back(path);
			}
		}
	}
}

//@brife передвижение стрелки для выбора файлом пользователем
int movingArrow(int ymin, int ymax, int cur, int key) {
	DWORD dw;
	COORD here{ 0, cur }; // координата стрелки в консоли (y - идет сверху вниз)
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) {
		printf("Invalid handle");
	}

	if (here.Y > ymin and key == 72) { // 72 - код клавиши стрелка вниз 
		// стирание бывшей стрелки и изменение ее координаты
		WriteConsoleOutputCharacter(hStdOut, L"  ", 2, here, &dw);
		here.Y -= 1;
	}
	if (here.Y < ymax and key == 80) { // 80 - код клавиши стрелка вниз 
		// стирание бывшей стрелки и изменение ее координаты
		WriteConsoleOutputCharacter(hStdOut, L"  ", 2, here, &dw);
		here.Y += 1;
	}

	WriteConsoleOutputCharacter(hStdOut, L">>", 2, here, &dw);

	return here.Y;
}

//@return возвращает название файла вместе с датой и времени для предотвращение перезаписи
std::string askName() {
	std::string filename = askString("введите название файла");

	filename = space2underscore(filename);
	filename = filename + "_" + currentTime();

	std::string fullPath = filename + ".html";

	system("cls");
	std::cout << "‘айл под названием " << filename << ".html создан!" << std::endl;
	return fullPath;
}

//@return замена всех пробелов на нижние подчеркивания
std::string space2underscore(std::string text) {
	std::replace(text.begin(), text.end(), ' ', '_');
	return text;
}

//@return получение времени на момент создания файла для предотвращения перезаписи
std::string currentTime() {
	std::string res;

	// получение времени на данный момент
	std::time_t t = std::time(NULL);
	std::tm now{};
	localtime_s(&now, &t);

	// преобразование времени в читаемый вид
	std::string date = formatXX(now.tm_mday) + "-"
		+ formatXX(now.tm_mon + 1) + "-"
		+ std::to_string(now.tm_year + 1900);


	std::string clockTime;

	clockTime = formatXX(now.tm_hour) + "-"
		+ formatXX(now.tm_min) + "-"
		+ formatXX(now.tm_sec);

	res = date + "--" + clockTime;
	return res;
}

//@return форматирование даты и времени
std::string formatXX(int num) {
	if (num < 10)
		return "0" + std::to_string(num);
	else
		return std::to_string(num);
}

