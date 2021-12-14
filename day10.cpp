#include "utils.hh"

int main() {
    string open = "([{<", close = ")]}>";
    int values[] = {3, 57, 1197, 25137};
    
    vec<llong> errors;
    vec<llong> scores;

    for (string line; std::getline(cin, line);) {
        vec<char> s;
        bool ok = true;
        for (char c : line) {
            if (utils::contains(open,c)) s.push_back(c);
            else if (s.size() > 0 && open.find(s.back()) == close.find(c)) s.pop_back();
            else {
                errors.push_back(values[close.find(c)]);
                ok = false;
                break;
            }
        }
        if (ok) {
            llong score = 0;
            for (char c : s | stdv::reverse) {
                score *= 5;
                score += open.find(c) + 1;
            }
            scores.push_back(score);
        }
    }

    cout << "Part 1: " << utils::sum(errors) << "\n"
         << "Part 2: " << utils::median(scores) << "\n";
}
