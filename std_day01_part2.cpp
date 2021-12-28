#include <iostream>
#include <iterator>
#include <vector>

int main() {
    using int_it = std::istream_iterator<int>;
    std::vector<int> input(int_it{std::cin}, int_it{});

    int count = 0;

    for (size_t i = 0; i < input.size() - 3; ++i) {
        count += input[i] < input [i + 3];
    }

    std::cout << "Part 2: " << count << "\n";
}
