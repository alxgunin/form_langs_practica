#include "ioputter.h"

TaskInput input() {
    TaskInput input;
    cin >> input.regexp >> input.x >> input.k;
    return input;
}

void output(TaskOutput output) {
    cout << output.answer << '\n';
}