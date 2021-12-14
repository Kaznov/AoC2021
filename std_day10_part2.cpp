#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

int main () {
    std::string open = "([{<", close = ")]}>";
    std::string line;
    std::vector<long> scores;

    while (std::getline(std::cin, line)) {
        std::vector<char> s;
        bool ok = true;
        for (char c : line) {
            if (open.find(c)!=std::string::npos) s.push_back(c);
            else if (s.size() > 0 && open.find(s.back()) == close.find(c)) s.pop_back();
            else {
                ok = false;
                break;
            }
        }
        if (ok) {
            long score = 0;
            for (char c : s | std::views::reverse) {
                score *= 5;
                score += open.find(c) + 1;
            }
            scores.push_back(score);
        }
    }

    std::ranges::nth_element(scores, scores.begin() + scores.size() / 2);
    std::cout << scores[scores.size() / 2];
}
