#include <iostream>
#include <locale>
#include <fstream>
using namespace std;

//Проверка существования файла
int CheckFile()
{
    std::ifstream fin("test.txt");

    //Файл существует
    if (fin.is_open())
    {
        fin.close();
        return 1;
    }
    return 0;   //файл не существует
}



int main()
{
    setlocale(LC_ALL, "Russian");
    
    //Проверка наличия файла с исходными данными
    if (CheckFile() == 0)
    {
        cout << "Исходный файл не найден.\nАвтоматическое закрытие программы.";
        return 0;
    }

    
}
