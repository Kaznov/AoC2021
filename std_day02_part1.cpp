#include <iostream>
#include <string>

int main() {
    std::string command;
    int value;
    int position = 0, depth = 0;

    while (std::cin >> command >> value) {
        if (command == "forward") {
            position += value;
        } else if (command == "down") {
            depth += value;
        } else {
            depth -= value;
        }
    }

    std::cout << "Part 1: " << position * depth << "\n";
}
