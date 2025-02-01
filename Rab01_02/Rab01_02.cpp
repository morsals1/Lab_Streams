// Rab01_02.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>

int pole[50];
int position = 0;
bool check_winner = false;

DWORD WINAPI Turtle_races(LPVOID iNum)
{
    int id = (int)iNum;
    int step = 0;


    for (int i = 0; i < id; i++) {
        step = rand() % (2 - 0 + 1) + 0;
    }

    while (position < 50) 
    {
        std::cout << " Это поток " << id << " шаг " << step << " позиция " << position << "\n";
        position += step;

        Sleep(1000);
    }

    return 0;

}

int main()
{
    setlocale(LC_ALL, "Russian");

    int turtle_sum;

    HANDLE theard[10];
    DWORD idTheard[10];

    std::cout << "Введите кол-во черепах от 5 до 10\n";
    std::cin >> turtle_sum;

    for (int i = 0; i < turtle_sum; i++) {
        theard[i] = CreateThread(NULL, 0, Turtle_races, (LPVOID)(i+1), 0, &idTheard[i]);

        if (theard[i] == NULL) {
            return GetLastError();
        }
        Sleep(1000);
    }

    int id_win = 0;

    for (int i = 0; i < turtle_sum; i++) {
        WaitForSingleObject(theard[i], INFINITE);
        CloseHandle(theard[i]);

        if (check_winner != true) {
            id_win = (int)theard[i];
        }
    }



    if (check_winner != true) {
        std::cout << " Это поток " << id_win << " позиция " << position << "ПОБЕДИТЕЛЬ!!!!" << "\n";
        check_winner == true;
    }



}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
