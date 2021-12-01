#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> input;
    std::copy(std::istream_iterator<int>{std::cin}, std::istream_iterator<int>{},
              std::back_inserter(input));
    std::vector<int> increases;
    std::adjacent_difference(std::begin(input), std::end(input),
                             std::back_inserter(increases),
                             std::greater<>{});
    increases.erase(increases.begin());
    int result = std::accumulate(std::begin(increases), std::end(increases), 0);
    std::cout << result;
}
