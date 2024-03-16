#ifndef DIRWORK
#define DIRWORK

#include "Sorting.h"

// функции, измен€ющие название файлов
std::string space2underscore(std::string text);
std::string currentTime();
std::string formatXX(int num);

// функци€, позвол€юща€ выбирать пользователю файлы
std::string findFile(std::string title);

// составные части findFile()
std::string askName();
int movingArrow(int ymin, int ymax, int cur, int key);
void fileChoice(int begCoord, std::vector<std::string> folders, int& cur, std::string& filepath);
void makeFilesList(std::string filepath, std::vector<std::string>& folderList);

#endif //DIRWORK