#include "slib.h"

TaskOutput GetAnswer(TaskInput input) {
    NFA nfa(input.regexp);
    std::vector<int> SufIds = GetSufIds(nfa, input.k, input.x);
    int32_t result = GetMinLength(nfa, SufIds);
    return TaskOutput(result);
}