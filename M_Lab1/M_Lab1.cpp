#include <iostream>
#include <locale>
#include <fstream>
using namespace std;


string FileName = "test.txt";
int num_str = 0;    //количество строк в файле

//Проверка существования файла
int CheckFile()
{
    std::ifstream fin(FileName);

    //Файл существует
    if (fin.is_open())
    {
        fin.close();
        return 1;
    }
    return 0;   //файл не существует
}

/*
//Подсчет количества строк в файле
void CheckFileFormat()
{
    ifstream file(FileName);
    char* str = new char[1024];
    int i = 0;
    while (!file.eof())
    {
        file.getline(str, 1024, '\n');
        i++;        //подсчет количества строк
    }
    num_str = i;
    file.close();       //закрытие файла
}
*/

//Чтение данных из файла
int ReadFile(double Matrix[][4], double MatrixB[][4])
{
    //Открытие файла
    ifstream file(FileName);
    double temp;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            file >> temp;
            Matrix[i][j] = temp;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            file >> temp;
            //cout << temp;
            MatrixB[i][j] = temp;
        }
    }
    
    return 1;
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

    double MatrixOrig[4][4];
    double MatrixB[3][4];

    ReadFile(MatrixOrig, MatrixB);
}
