#include <algorithm>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
#include <queue>

const int32_t INVALID = -123;
const int32_t INF = 2e9;
const char EPSILON = '#';
using std::pair;
using std::vector;
using std::stack;
using std::string;
using std::queue;
using std::min;
typedef pair<int32_t, int32_t> pii;
typedef pair<int32_t, int32_t> pic;

class TaskInput {
public:
    string regexp;
    char x;
    int32_t k;
};

class TaskOutput {
public:
    string answer;
    TaskOutput(int32_t);
};

class NFA {
private:
    int32_t vertex_count;
    vector<vector<pic>> graph;
    vector<vector<pic>> graphReversed;
    vector<bool> is_start;
    vector<bool> is_end;
    void RemoveEpsilonsDfs(int32_t, int32_t,vector<bool>&);
    void ParseAndMakeGraph(string);
    void RemoveEpsilons();
    void MakeReversedGraph();
public:
    NFA(string);
    ~NFA() = default;
    friend vector<int32_t> GetSufIds(NFA&, int32_t, char);
    friend int32_t GetMinLength(NFA&, vector<int32_t>&);
};

std::vector<int> GetSufIds(NFA&, int32_t, char);

int32_t GetMinLength(NFA&, vector<int32_t>&);

TaskOutput GetAnswer(TaskInput);