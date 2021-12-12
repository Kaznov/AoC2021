#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string open = "([{<", close = ")]}>";
    int values[] = {3, 57, 1197, 25137};
    std::string line;
    int err = 0;
    while (std::getline(std::cin, line)) {
        std::vector<char> s;
        bool ok = true;
        for (char c : line) {
            if (open.contains(c)) s.push_back(c);
            else if (s.size() > 0 && open.find(s.back()) == close.find(c)) s.pop_back();
            else {
                err += values[close.find(c)];
                break;
            }
        }
    }

    std::cout << err;
}
