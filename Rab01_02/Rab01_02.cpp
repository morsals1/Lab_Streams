#include <iostream>
#include <Windows.h>
#include <cstdio>

int positions[10] = { 0 };
bool check_winner = false;
int winner_id = -1;
FILE* logFile;

DWORD WINAPI Turtle_races(LPVOID iNum)
{
    int id = (int)iNum;
    int current_position = 0;
    int step_count = 0;

    srand(GetTickCount() + id * 100);

    while (current_position < 50)
    {
        int step = rand() % 3;
        current_position += step;
        step_count++;

        if (current_position >= 50) {
            break;
        }

        std::cout << "Поток " << id << " шаг " << step << " позиция " << current_position << "\n";

        fprintf(logFile, "Поток %d, шаг %d, позиция %d\n", id, step, current_position);
        fflush(logFile);

        Sleep(1000);
        positions[id - 1] = current_position;

        if (current_position == 49 && !check_winner) {
            check_winner = true;
            winner_id = id;
            break;
        }
    }

    return 0;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    int turtle_sum;

    if (fopen_s(&logFile, "race_log.txt", "w") != 0) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return 1;
    }

    HANDLE threads[10];
    DWORD threadID[10];

    std::cout << "Введите кол-во черепах от 5 до 10\n";
    std::cin >> turtle_sum;

    if (turtle_sum < 5 || turtle_sum > 10) {
        std::cout << "Неверное число черепах! Введите число от 5 до 10.\n";
        fclose(logFile);
        return 1;
    }

    for (int i = 0; i < turtle_sum; i++) {
        threads[i] = CreateThread(NULL, 0, Turtle_races, (LPVOID)(i + 1), 0, &threadID[i]);

        if (threads[i] == NULL) {
            fclose(logFile);
            return GetLastError();
        }
    }

    WaitForMultipleObjects(turtle_sum, threads, TRUE, INFINITE);

    for (int i = 0; i < turtle_sum; i++) {
        CloseHandle(threads[i]);
    }

    if (check_winner) {
        std::cout << "Победитель: Поток с номером " << winner_id << "\n";
        fprintf(logFile, "Победитель: Поток с номером %d\n", winner_id);
    }
    else {
        std::cout << "Нет победителя.\n";
        fprintf(logFile, "Нет победителя.\n");
    }

    fclose(logFile);
    return 0;
}
