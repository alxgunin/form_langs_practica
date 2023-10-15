#include "ioputter.h"

TaskInput input() {
    TaskInput input;
    std::cin >> input.regexp >> input.x >> input.k;
}

void output(TaskOutput output) {
    std::cout << output.answer << '\n';
}