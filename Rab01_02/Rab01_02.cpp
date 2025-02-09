#include <iostream>
#include <Windows.h>
#include <cstdio>

const int TRACK_LENGTH = 50;
const int MAX_TURTLES = 10;

char race_tracks[MAX_TURTLES][TRACK_LENGTH + 1];
FILE* logFile;
int winner_id = -1;

DWORD WINAPI Turtle_races(LPVOID param) {
    int id = (int)(uintptr_t)param;
    int current_position = 0;
    srand(GetTickCount() + id * 100);

    while (current_position < TRACK_LENGTH - 1) {
        int step = rand() % 3;
        current_position += step;
        if (current_position >= TRACK_LENGTH - 1) {
            current_position = TRACK_LENGTH - 1;
        }

        race_tracks[id - 1][current_position] = 'T';

        std::cout << "Поток " << id << " шаг " << step << " позиция " << current_position << "\n";

        fopen_s(&logFile, "race_log.txt", "a");
        if (logFile) {
            fprintf(logFile, "Поток %d, шаг %d, позиция %d\n", id, step, current_position);
            fclose(logFile);
        }

        Sleep(1000 + rand() % 2000);

        if (current_position >= TRACK_LENGTH - 1 && winner_id == -1) {
            winner_id = id;
        }

        if (current_position - step >= 0) {
            race_tracks[id - 1][current_position - step] = ' ';
        }
    }

    return 0;
}

int main() {
    setlocale(LC_ALL, "Russian");

    int turtle_sum;
    std::cout << "Введите количество черепах (от 5 до 10): ";
    std::cin >> turtle_sum;

    if (turtle_sum < 5 || turtle_sum > MAX_TURTLES) {
        std::cerr << "Неверное число черепах! Введите от 5 до 10.\n";
        return 1;
    }

    fopen_s(&logFile, "race_log.txt", "w");
    if (logFile) fclose(logFile);

    for (int i = 0; i < turtle_sum; i++) {
        for (int j = 0; j < TRACK_LENGTH; j++) {
            race_tracks[i][j] = ' ';
        }
        race_tracks[i][TRACK_LENGTH] = '\0';
    }

    HANDLE threads[MAX_TURTLES];

    for (int i = 0; i < turtle_sum; i++) {
        threads[i] = CreateThread(NULL, 0, Turtle_races, (LPVOID)(uintptr_t)(i + 1), 0, NULL);
        if (threads[i] == NULL) {
            std::cerr << "Ошибка создания потока " << i + 1 << "\n";
            return GetLastError();
        }
    }

    WaitForMultipleObjects(turtle_sum, threads, TRUE, INFINITE);

    for (int i = 0; i < turtle_sum; i++) {
        CloseHandle(threads[i]);
    }

    fopen_s(&logFile, "race_log.txt", "a");
    if (logFile) {
        if (winner_id != -1) {
            std::cout << "Победитель: Поток " << winner_id << "\n";
            fprintf(logFile, "Победитель: Поток %d\n", winner_id);
        }
        fclose(logFile);
    }

    return 0;
}