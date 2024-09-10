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

void PrintVector(double Vector[4])
{
    for (int i = 0; i < 4; i++)
    {
        cout << Vector[i] << endl;
    }
}



void Y(double MatrixLU[][4], double MatrixB[][4], int k, double vectorY[4])
{
    for (int i = 0; i < 4; i++)
    {
        double temp = 0;

        for (int j = 0; j < i; j++)
        {
            temp = temp + vectorY[j] * MatrixLU[i][j];
        }

        vectorY[i] = MatrixB[k][i] - temp;
    }
}


void X(double MatrixLU[][4], double VectorY[4], double VectorX[4])
{
    for (int i = 4 - 1; i >= 0; i--)
    {
        double temp = 0;

        for (int j = i + 1; j < 4; j++)
        {
            temp = temp + MatrixLU[i][j] * VectorX[j];
        }

        VectorX[i] = (VectorY[i] - temp) / MatrixLU[i][i];
    }
}

void Residuals(double Matrix[][4], double MatrixB[][4], double vectorX[4], int k)
{
    double res[4];

    for (int i = 0; i < 4; i++)
    {
        res[i] = MatrixB[k][i];

        for (int j = 0; j < 4; j++)
        {
            res[i] -= Matrix[i][j] * vectorX[j];
        }
    }

    PrintVector(res);
}

void ReverseSearch(double MatrixOrig[][4], double MatrixReverse[][4])
{
    for (int k = 3; k >= 0; k--)
    {
        double sum = 0;

        for (int n = k + 1; n < 4; n++)
        {
            sum += MatrixOrig[k][n] * MatrixReverse[n][k];
        }

        MatrixReverse[k][k] = (1 - sum) / MatrixOrig[k][k];

        for (int i = k - 1; i >= 0; i--)
        {
            sum = 0;

            for (int n = i + 1; n < 4; n++)
            {
                sum += MatrixOrig[i][n] * MatrixReverse[n][k];
            }

            MatrixReverse[i][k] = (-1) * sum / MatrixOrig[i][i];
        }

        for (int j = k - 1; j >= 0; j--)
        {
            sum = 0;

            for (int n = j + 1; n < 4; n++)
            {
                sum += MatrixReverse[k][n] * MatrixOrig[n][j];
            }

            MatrixReverse[k][j] = (-1) * sum;
        }
    }
}


void DecompLU(double Matrix[][4], double MatrixLU[][4])
{
    for (int i = 0; i < 4; i++)
    {
        MatrixLU[0][i] = Matrix[0][i];
    }

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

            MatrixLU[j][i] = (Matrix[j][i] - temp) / MatrixLU[i][i];
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

            MatrixLU[i][j] = Matrix[i][j] - temp;
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

    double Matrix[4][4], MatrixLU[4][4], MatrixReverse[4][4];
    double MatrixB[3][4];
    double VectorX[4], VectorY[4];

    ReadFile(Matrix, MatrixB);

    cout << "Матрица A" << endl;
    PrintMatrix(3, Matrix);

    cout << endl << "Матрица L - E + U" << endl;

    DecompLU(Matrix, MatrixLU);
    PrintMatrix(3, MatrixLU);

    double det = 1;

    for (int k = 0; k < 4; k++)
    {
        det = det * MatrixLU[k][k];
    }

    cout << endl << "Определитель |A| = " << det << endl;

    cout << endl << "Обратная матрица" << endl;
    ReverseSearch(MatrixLU, MatrixReverse);
    PrintMatrix(3, MatrixReverse);

    for (int k = 0; k < 3; k++)
    {
        cout << endl << "Тест №" << k + 1 << endl;

        cout << endl << "Вектор B" << endl;
        PrintMatrix(k, MatrixB);

        cout << endl << "Вектор Y" << endl;

        Y(MatrixLU, MatrixB, k, VectorY);
        PrintVector(VectorY);

        cout << endl << "Вектор X" << endl;

        X(MatrixLU, VectorY, VectorX);
        PrintVector(VectorX);

        cout << endl << "Невязки" << endl;

        Residuals(Matrix, MatrixB, VectorX, k);
        cout << endl;
    }
}
