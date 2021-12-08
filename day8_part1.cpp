#include <iostream>
#include <string>

struct Display {
    std::string input[10];
    std::string output[4];
};

Display readLine() {
    Display d;
    for (int i = 0; i < 10; ++i)
        std::cin >> d.input[i];
    char sep;
    std::cin >> sep;
    for (int i = 0; i < 4; ++i)
        std::cin >> d.output[i];
    std::cin.ignore(16, '\n');
    return d;
}

int main() {
    int count1478 = 0;

    while (std::cin.peek() && std::cin) {
        Display d = readLine();
        for (auto&& o : d.output)
            count1478 += o.size() == 2 || o.size() == 3 ||
                         o.size() == 4 || o.size() == 7;
    }

    std::cout << count1478;
}
