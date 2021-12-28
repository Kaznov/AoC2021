#include <algorithm>
#include <cstdio>
#include <numeric>
#include <type_traits>
#include <vector>

int main() {
    std::vector<int> crabs;
    int next_crab;

    while (scanf("%d,", &next_crab) == 1) {
        crabs.push_back(next_crab);
    }

    auto mid = std::begin(crabs) + std::size(crabs) / 2;
    std::nth_element(std::begin(crabs),
                     mid,
                     std::end(crabs));
    // there are 2 median elements in input, so no need for average
    int median = *mid;

    int cost = std::accumulate(std::begin(crabs),
                               std::end(crabs),
                               0,
                               [=](int acc, int next) {
                                   return acc + abs(next - median);});

    printf("%d", cost);
}
