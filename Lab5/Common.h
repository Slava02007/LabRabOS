#pragma once
#include <windows.h>

struct employee {
    int num;
    char name[10];
    double hours;
};

enum Command {
    CMD_CHECK_ID,   // Запрос на чтение записи
    CMD_UPDATE,     // Запрос на обновление записи
    CMD_END_ACCESS, // Завершение доступа к записи
    CMD_EXIT        // Выход из программы
};

struct Request {
    Command cmd;
    int id;
    employee data;
};

struct Response {
    bool found;
    employee data;
};

const wchar_t* PIPE_NAME = L"\\\\.\\pipe\\MyLaboratornayaPipe";