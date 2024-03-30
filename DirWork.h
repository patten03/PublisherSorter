#ifndef DIRWORK
#define DIRWORK

#include "Sorting.h"

// функции, измен€ющие название файлов
std::string space2underscore(std::string text); // замена пробелов в названии файлах на нижние подчеркивани€
std::string currentTime();                      // получение времени на данный момент дл€ предотвращени€ перезаписи файлов
std::string formatXX(int num);                  // преобразование чисел дл€ времени и дат

// функци€, позвол€юща€ выбирать пользователю файлы
std::string findFile(std::string title);
    
// составные части findFile()
std::string askName(); // получение название файла от пользовател€
int movingArrow(int ymin, int ymax, int cur, int key);                                            // движение стрелки выбора
void fileChoice(int begCoord, std::vector<std::string> folders, int& cur, std::string& filepath); // выбор файла
void makeFilesList(std::string filepath, std::vector<std::string>& folderList);                   //создание списка файлов

#endif //DIRWORK