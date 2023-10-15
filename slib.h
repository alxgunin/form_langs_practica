#include <string>
#include <vector>

class TaskInput {
public:
    std::string regexp;
    char x;
    int32_t k;
};

class TaskOutput {
public:
    std::string answer;
    TaskOutput(int32_t);
};

class NFA {
private:
    std::vector<int32_t> state;
    std::vector<std::pair<int32_t, char>> grapth;
public:
    NFA(std::string val);
};

std::vector<int> GetSufIds(NFA&, int32_t, char);

int32_t GetMinLength(NFA&, std::vector<int>&);

TaskOutput GetAnswer(TaskInput);