#include <iostream>
#include <string>

int main() {
    std::string command;
    int value;
    long long int position = 0, depth = 0, aim = 0;

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

    std::cout << "Part 2: " << position * depth << "\n";
}
