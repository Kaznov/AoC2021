#include <algorithm>
#include <cstdio>
#include <numeric>
#include <type_traits>
#include <vector>

template <typename RandomIt>
auto median(RandomIt begin, RandomIt end) {
    auto size = end - begin;
    if (size == 0)
        return std::remove_cvref_t<decltype(*begin)>{};

    std::vector temp_buffer(begin, end);
    auto tbegin = temp_buffer.begin();
    auto tend = temp_buffer.end();
    if (size % 2 == 1) {
        auto mid = tbegin + size / 2;
        std::nth_element(tbegin, mid, tend);
        return std::move(*mid);
    }
    else {
        auto mid = tbegin + size / 2 - 1;
        std::nth_element(tbegin, mid, tend);
        std::iter_swap(
            mid + 1,
            std::min_element(mid + 1, tend));
        return (std::move(*mid) + std::move(*(mid + 1))) / 2;
    }
}

int main() {
    std::vector<int> crabs;
    int next_crab;

    while (scanf("%d,", &next_crab) == 1) {
        crabs.push_back(next_crab);
    }

    int median_v = median(std::begin(crabs), std::end(crabs));
    int cost = std::accumulate(std::begin(crabs),
                               std::end(crabs),
                               0,
                               [=](int acc, int next) {
                                   return acc + abs(next - median_v);});

    printf("%d", cost);
}
