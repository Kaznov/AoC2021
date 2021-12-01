#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> input;
    std::copy(std::istream_iterator<int>{std::cin}, std::istream_iterator<int>{},
              std::back_inserter(input));
    int result = 0;
    int last_moving_sum = 1000000;
    for (size_t i = 0; i < input.size() - 2; ++i) {
        int sum = input[i] + input[i + 1] + input[i + 2];
        if (sum > last_moving_sum) ++result;
        last_moving_sum = sum;
    }
    std::cout << result;
}
