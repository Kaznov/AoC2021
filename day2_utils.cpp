#include "utils.hh"

int main() {
    string command;
    int value;
    int position = 0, aim = 0, depth = 0;
    while (std::cin >> command >> value) {
        if (command == "forward") {
            position += value;
            depth += value * aim;
        } else if (command == "down") {
            aim += value;
        } else {
            aim -= value;
        }
    }
    std::cout << "Part 1: " << position * aim << "\n"
              << "Part 2: " << position * depth << "\n";
}
