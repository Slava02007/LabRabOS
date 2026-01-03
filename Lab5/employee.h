#pragma once
struct employee {
    int num;
    char name[10];
    double hours;
};

enum Operation { READ, MODIFY, SAVE, EXIT };

struct Request {
    Operation op;
    int id;
    employee data;
};