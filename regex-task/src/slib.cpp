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
    if (is_end[vertex]) {
        is_end[parent] = true;
    }
    if (is_start[vertex]) {
        is_start[parent] = true;
    }
    if (used[vertex]) return;
    used[vertex] = true;
    for (size_t i = 0; i < graph[vertex].size(); ++i) {
        auto& [next_vertex, symbol] = graph[vertex][i];
        if (symbol == EPSILON) {
            if (next_vertex != INVALID) {
                RemoveEpsilonsDfs(next_vertex, vertex, used);
            }
            graph[vertex][i].first = INVALID;
        } else {
            if (parent != vertex) {
                graph[parent].push_back(pic(next_vertex, symbol));
            }
        }
    }
    if (vertex == parent) {
        for (auto& [next_vertex, symbol] : graph[vertex]) {
            if (symbol == EPSILON && next_vertex != INVALID) {
                next_vertex = INVALID;
            }
        }
    }
}

void NFA::RemoveEpsilons() {
    for (int32_t vertex = 0; vertex < vertex_count; ++vertex) {
        vector<bool> used(vertex_count);
        RemoveEpsilonsDfs(vertex, vertex, used);
    }
    for (int32_t vertex = 0; vertex < vertex_count; ++vertex) {
        for (auto& [next_vertex, symbol] : graph[vertex]) {
            if (next_vertex != INVALID) {
            }
        }
    }
}   

void NFA::MakeReversedGraph() {
    graphReversed.resize(vertex_count);
    for (int32_t vertex = 0; vertex < vertex_count; ++vertex) {
        for (auto& [next_vertex, symbol] : graph[vertex]) {
            if (next_vertex != INVALID) {
                graphReversed[next_vertex].push_back(pic(vertex, symbol));
            }
        }
    }
}


NFA::NFA(string regexp) {
    ParseAndMakeGraph(regexp);
    RemoveEpsilons();
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
    NFA nfa(input.regexp);
    vector<int> SufIds = GetSufIds(nfa, input.k, input.x);
    int32_t result = GetMinLength(nfa, SufIds);
    result = (result == -1 ? -1 : result + input.k);
    return TaskOutput(result);
}