#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <algorithm>

typedef struct {
    int states = 0;
    std::string alphabet;
    int init_state = 0;
    std::vector<int> final_states;
    std::vector<std::vector<char>> rules;
}automata;

bool belongsTo(int i, std::vector<int> const & v) {
    return std::find(v.begin(), v.end(), i) != v.end();
}

bool belongsTo(char i, std::vector<char> const& v) {
    return std::find(v.begin(), v.end(), i) != v.end();
}

bool haveCommon(std::vector<int> const & v1, std::vector<int> const & v2) {
    for (auto& i : v1) {
        if (belongsTo(i, v2)) return true;
    }

    return false;
}

std::vector<int> join(std::vector<int> const & v1, std::vector<int> const & v2) {
    std::vector<int> newv(v1);
    for (auto& i : v2) {
        if (!belongsTo(i, newv)) newv.push_back(i);
    }
    return newv;
}

std::vector<int> getLambda(std::vector<int> const & states, int maxStates, std::vector<std::vector<char>> const & rules) {
    std::vector<int> newstates;

    for (auto& state : states) {
        for (int i = 0; i < maxStates; i++) {
            if (belongsTo('$', rules[state * maxStates + i]) && !belongsTo(i, newstates)) {
                newstates.push_back(i);
            }
        }
    }

    return newstates;
}

std::vector<int> getstates(char c, std::vector<int> const & states, int maxStates, std::vector<std::vector<char>> const & rules) {
    std::vector<int> newstates;

    for (auto& state : states) {
        for (int i = 0; i < maxStates; i++) {
            if (belongsTo(c, rules[state * maxStates + i]) && !belongsTo(i, newstates)) {
                newstates.push_back(i);
            }
        }
    }

    std::vector<int> lambdas;
    bool hasNew;

    do {
        lambdas = getLambda(newstates, maxStates, rules);
        hasNew = false;
        for (auto& i : lambdas) {
            if (!belongsTo(i, newstates)) {
                hasNew = true;
                break;
            }
        }

        newstates = join(newstates, lambdas);
    } while (hasNew);
    
    return newstates;
}

bool evaluate(automata& a, std::string s) {
    std::vector<int> curr_states;
    curr_states.push_back(a.init_state);

    std::vector<int> lambdas = getLambda(curr_states, a.states, a.rules);
    curr_states = join(curr_states, lambdas);
    
    while (s != "") {
        curr_states = getstates(s.front(), curr_states, a.states, a.rules);
        s.erase(s.begin());
    }

    if (haveCommon(a.final_states, curr_states))
        return true;
    return false;
}

void read_automata(automata& a, std::string input) {
    std::fstream f;
    f.open(input, std::ios::in);

    f >> a.states;

    int n; //counter

    f >> n; //characters in alphabet
    for (int i = 0; i < n; i++) {
        char c;
        f >> c;
        a.alphabet += c;
    }

    f >> a.init_state;

    f >> n; //nr of final states
    for (int i = 0; i < n; i++) {
        int state;
        f >> state;
        a.final_states.push_back(state);
    }

    a.rules.resize(a.states * a.states);

    f >> n; //nr of transitions
    for (int i = 0; i < n; i++) {
        int x, y;
        char c;
        f >> x;
        f >> c;
        f >> y;

        a.rules[x * a.states + y].push_back(c);
    }

    f.close();
}

int main() {
    automata a;
    read_automata(a, "input.txt");

    std::string s;
    std::cout << "s = ";
    std::cin >> s;

    if(evaluate(a, s))
        std::cout << "DA" << std::endl;
    else
        std::cout << "NU" << std::endl;

    return 0;
}
