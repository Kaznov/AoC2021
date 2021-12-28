#include <iostream>
#include <ranges>

namespace stdr = std::ranges;

int main() {
    int last_value = 1000000;  // greater than any input
    int count = 0;

    for (int v : stdr::istream_view<int>(std::cin)) {
        if (v > last_value) ++count;
        last_value = v;
    }

    std::cout << "Part 1: " << count << "\n";
}
