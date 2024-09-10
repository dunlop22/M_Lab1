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

void PrintMatrix(int num, double Matrix[][4])
{
    if (num < 3)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << Matrix[num][j] << "\n";
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                cout << Matrix[i][j] << " ";
            }
            cout << "\n";
        }
    }
}

void Y(double MatrixLU[][4], double MatrixB[][4])
{
    for (int i = 0; i < 4; i++)
    {
        double temp = 0;

        for (int j = 0; j < i; j++)
        {
            temp = temp + MatrixB[0][j] * MatrixLU[i][j];
        }
        MatrixB[0][i] = MatrixB[0][i] - temp;
    }
}


void X(double MatrixOrig[][4], double MatrixB[][4])
{
    double result[4];

    for (int i = 4 - 1; i >= 0; i--)
    {
        double temp = 0;

        for (int j = i + 1; j < 4; j++)
        {
            temp = temp + MatrixOrig[i][j] * result[j];
        }

        result[i] = (MatrixB[0][i] - temp) / MatrixOrig[i][i];
    }
    
    for (int i = 0; i < 4; i++)
    {
        cout << result[i];
    }
}


void func(double MatrixLU[][4])
{
    for (int i = 0; i < 4;)
    {
        if (MatrixLU[i][i] == 0)
        {
            cout << "LU разложение невозможно";
            return;
        }


        //L
        for (int j = i + 1; j < 4; j++)
        {
            double temp = 0;

            for (int m = 0; m < i; m++)
            {
                temp = temp + MatrixLU[j][m] * MatrixLU[m][i];
            }

            MatrixLU[j][i] = (MatrixLU[j][i] - temp) / MatrixLU[i][i];
        }
        i++;
        //U
        for (int j = i; j < 4; j++)
        {
            double temp = 0;

            for (int m = 0; m < i; m++)
            {
                temp = temp + MatrixLU[i][m] * MatrixLU[m][j];
            }

            MatrixLU[i][j] = MatrixLU[i][j] - temp;
        }
    }
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
    PrintMatrix(3, MatrixOrig);

    func(MatrixOrig);
    PrintMatrix(3, MatrixOrig);

    Y(MatrixOrig, MatrixB);
    PrintMatrix(0, MatrixB);

    X(MatrixOrig, MatrixB);

}
