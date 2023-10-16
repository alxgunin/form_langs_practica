#include "slib.hpp"

TaskOutput::TaskOutput(int32_t value) {
    if (value == -1) {
        answer = "INF";
    } else {
        answer = std::to_string(value);
    }
}

void NFA::ParseAndMakeGraph(string regexp) {
    vertex_count = 0;
    stack<pair<int32_t, int32_t>> stack;
    int32_t begin1, begin2, end1, end2;
    for (char cur : regexp) {
        // std::cerr << "char is " << cur << std::endl;
        switch (cur)
        {
        case '+':
            if (stack.size() < 2) {
                string error = "Incorrect regex: too few argumets for operation \"+\", expected 2, got " + std::to_string(stack.size());
                throw std::runtime_error(error);
            }
            begin1 = stack.top().first;
            end1 = stack.top().second;
            stack.pop();
            begin2 = stack.top().first;
            end2 = stack.top().second;
            stack.pop();
            graph.resize(vertex_count + 2);
            is_start.resize(vertex_count + 2);
            is_end.resize(vertex_count + 2);
            graph[vertex_count].push_back(pic(begin1, EPSILON));
            graph[vertex_count].push_back(pic(begin2, EPSILON));
            graph[end1].push_back(pic(vertex_count + 1, EPSILON));
            graph[end2].push_back(pic(vertex_count + 1, EPSILON));
            is_start[begin1] = is_end[end1] = is_start[begin2] = is_end[end2] = false;
            is_start[vertex_count] = true;
            is_end[vertex_count + 1] = true;
            vertex_count += 2;
            break;  
        case '.':
            if (stack.size() < 2) {
                string error = "Incorrect regex: too few argumets for operation \".\", expected 2, got " + std::to_string(stack.size());
                throw std::runtime_error(error);
            }
            begin2 = stack.top().first;
            end2 = stack.top().second;
            stack.pop();
            begin1 = stack.top().first;
            end1 = stack.top().second;
            stack.pop();
            graph[end1].push_back(pic(begin2, EPSILON));
            is_end[end1] = is_start[begin2] = false;
            break;
        case '*':
            if (stack.size() < 1) {
                throw std::runtime_error("Incorrect regex: too few argumets for operation \"*\", expected 1, got 0");
            }
            begin1 = stack.top().first;
            end1 = stack.top().second;
            stack.pop();
            graph.resize(vertex_count + 1);
            is_start.resize(vertex_count + 1);
            is_end.resize(vertex_count + 1);
            graph[vertex_count].push_back(pic(begin1, EPSILON));
            is_start[begin1] = is_end[end1] = false;
            graph[end1].push_back(pic(vertex_count, EPSILON));
            is_start[vertex_count] = is_end[vertex_count] = true;
            break;
        default:
            graph.resize(vertex_count + 2);
            is_start.resize(vertex_count + 2);
            is_end.resize(vertex_count + 2);
            graph[vertex_count].push_back(pic(vertex_count + 1, cur));
            is_start[vertex_count] = true;
            is_end[vertex_count + 1] = true;
            stack.push(pii(vertex_count, vertex_count + 1));
            vertex_count += 2;
        }
    }
    if (stack.size() > 1) {
        throw std::runtime_error("Incorrect regex: cannot parse more than one char");
    }
}

void NFA::RemoveEpsilonsDfs(int32_t vertex, int32_t parent, vector<bool>& used) {
    // std::cerr << "Launch from " << vertex << " node, parent node " << parent << std::endl;
    // std::cerr << "if#1..." << std::endl;
    if (is_end[vertex]) {
        is_end[parent] = true;
    }
    if (is_start[vertex]) {
        is_start[parent] = true;
    }
    // std::cerr << "if#2..." << std::endl;
    if (used[vertex]) return;
    used[vertex] = true;
    // std::cerr << "Going down..." << std::endl;
    for (size_t i = 0; i < graph[vertex].size(); ++i) {
        auto& [next_vertex, symbol] = graph[vertex][i];
        // std::cerr << vertex << ' ' << next_vertex << ' ' << symbol << std::endl;
        if (symbol == EPSILON) {
            if (next_vertex != INVALID) {
                RemoveEpsilonsDfs(next_vertex, vertex, used);
            }
            next_vertex = INVALID;
        } else {
            if (parent != vertex) {
                graph[parent].push_back(pic(next_vertex, symbol));
                // std::cerr << "pushed " << next_vertex << " to " << parent << std::endl;
            }
        }
    }
    // std::cerr << "second" << std::endl;
    if (vertex == parent) {
        for (auto& [next_vertex, symbol] : graph[vertex]) {
            if (symbol == EPSILON && next_vertex != INVALID) {
                next_vertex = INVALID;
            }
        }
    }
    // std::cerr << "The end of DFS" << std::endl;
}

void NFA::RemoveEpsilons() {
    for (int32_t vertex = 0; vertex < vertex_count; ++vertex) {
        // std::cerr << "iteration " << vertex << std::endl;
        vector<bool> used(vertex_count);
        RemoveEpsilonsDfs(vertex, vertex, used);
        // std::cerr << "got out from dfs" << std::endl;
    }
    std::cerr << "REMOVED\n";
    for (int32_t vertex = 0; vertex < vertex_count; ++vertex) {
        // std::cerr << "vertex " << vertex << std::endl;
        // std::cerr << "connected to ";
        for (auto& [next_vertex, symbol] : graph[vertex]) {
            if (next_vertex != INVALID) {
                // std::cerr << next_vertex << ' ';
            }
        }
        // std::cerr << '\n';
    }
    std::cerr << "passed" << std::endl;
    vector<vector<pic>> temp;
    std::cerr << "passed" << std::endl;
    temp = graph;
    std::cerr << "passed" << std::endl;
    graph.clear();
    std::cerr << "passed" << std::endl;
    graph.resize(vertex_count);
    std::cerr << "passed" << std::endl;
    for (int32_t vertex = 0; vertex < vertex_count; ++vertex) {
        for (auto& [next_vertex, symbol] : temp[vertex]) {
            if (next_vertex != INVALID) {
                // std::cerr << "condended graph is " << vertex << " to " << next_vertex << " by " << static_cast<char>(symbol) << std::endl;
                graph[vertex].push_back(pic(next_vertex, symbol));
            }
        }
    }
    std::cerr << "start from" << std::endl;
    for (int32_t vertex = 0; vertex < vertex_count; ++vertex) {
        if (is_start[vertex]) {
            std::cerr << vertex << ' ';
        }
    }
    std::cerr << std::endl;
    std::cerr << "end from" << std::endl;
    for (int32_t vertex = 0; vertex < vertex_count; ++vertex) {
        if (is_end[vertex]) {
            std::cerr << vertex << ' ';
        }
    }
    std::cerr << std::endl;

}   

void NFA::MakeReversedGraph() {
    graphReversed.resize(vertex_count);
    for (int32_t vertex = 0; vertex < vertex_count; ++vertex) {
        for (auto& [next_vertex, symbol] : graph[vertex]) {
            graphReversed[next_vertex].push_back(pic(vertex, symbol));
        }
    }
}


NFA::NFA(string regexp) {
    ParseAndMakeGraph(regexp);
    std::cerr << "parse passed!\n";
    RemoveEpsilons();
    std::cerr << "remove passed!\n";
    MakeReversedGraph();
}

vector<int> GetSufIds(NFA& nfa, int32_t k, char x) {
    vector<vector<int32_t>> achievable(nfa.vertex_count, vector<int32_t>(k + 1));
    for (int32_t vertex = 0; vertex < nfa.vertex_count; ++vertex) {
        if (nfa.is_end[vertex]) {
            achievable[vertex][0] = 1;
        }
    }
    vector<int> res;
    for (int32_t dist = 0; dist < k; ++dist) {
        for (int32_t vertex = 0; vertex < nfa.vertex_count; ++vertex) {
            for (auto& [next_vertex, symbol] : nfa.graphReversed[vertex]) {
                if (symbol == x) {
                    achievable[next_vertex][dist + 1] |= achievable[vertex][dist];
                }
            }
        }
    }
    for (int32_t vertex = 0; vertex < nfa.vertex_count; ++vertex) {
        if (achievable[vertex][k]) {
            res.push_back(vertex);
        }
    }
    return res;
}

int32_t GetMinLength(NFA& nfa, vector<int32_t>& SufIds) {
    if (SufIds.empty()) {
        return -1;
    }
    queue<int32_t> queue;
    vector<int32_t> dist(nfa.vertex_count, INF);
    for (auto vertex : SufIds) {
        queue.push(vertex);
        dist[vertex] = 0;
    }
    while(!queue.empty()) {
        int32_t vertex = queue.front();
        queue.pop();
        for (auto& [next_vertex, symbol] : nfa.graphReversed[vertex]) {
            if (dist[next_vertex] > dist[vertex] + 1) {
                dist[next_vertex] = dist[vertex] + 1;
                queue.push(next_vertex);
            }
        }
    }
    int32_t length = INF;
    for (int32_t vertex = 0; vertex < nfa.vertex_count; ++vertex) {
        if (nfa.is_start[vertex]) {
            length = min(length, dist[vertex]);
        }
    }
    return length;
}

TaskOutput GetAnswer(TaskInput input) {
    std::cerr << "started!\n";
    NFA nfa(input.regexp);
    std::cerr << "made nfa!\n";
    vector<int> SufIds = GetSufIds(nfa, input.k, input.x);
    std::cerr << "got sufids!\n";
    int32_t result = GetMinLength(nfa, SufIds);
    std::cerr << "got length!\n";
    result = (result == -1 ? -1 : result + input.k);
    std::cerr << result << '\n';
    return TaskOutput(result);
}