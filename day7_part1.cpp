#include <algorithm>
#include <numeric>
#include <type_traits>
#include <vector>

template <typename T>
class median_result {
    public:
    inline bool empty() const {
        return state_ == state::empty;
    }

    inline bool distinct() const {
        return state_ == state::one_value;
    }

    inline operator T() const {
        return empty() ? T{}
                : distinct() ? low_
                : std::midpoint(low_, high_);
    }

    inline T low() const {
        return low_;
    }

    inline T high() const {
        return high_;
    }

    template <typename RandomIt, typename Compare>
    friend auto median(RandomIt begin, RandomIt end, Compare comp)
        -> median_result<
            std::remove_cv_t<std::remove_reference_t<decltype(*begin)>>>

    private:
    median_result() : state_{state::empty} {}
    median_result(T&& low)
        : state_{state::one_value}, low_{std::move(low)} {}
    median_result(T&& low, T&& high)
        : state_{state::two_values}, low_{std::move(low)}, high_{std::move(high)} {}

    enum class state : char {
        empty,
        one_value,
        two_values
    } state_;
    T low_, high_;
};

template <typename RandomIt, typename Compare>
auto median(RandomIt begin, RandomIt end, Compare comp)
        -> median_result<
            std::remove_cv_t<std::remove_reference_t<decltype(*begin)>>> {
    auto size = end - begin;
    if (size == 0) {
        return {};
    }

    std::vector temp_buffer(begin, end);
    auto tbegin = temp_buffer.begin();
    auto tend = temp_buffer.end();
    if (size % 2 == 1) {
        auto mid = tbegin + size / 2;
        std::nth_element(tbegin, mid, tend, comp);
        return {std::move(*mid)};
    }
    else {
        auto mid = tbegin + size / 2 - 1;
        std::nth_element(tbegin, mid, tend, comp);
        std::iter_swap(
            mid + 1,
            std::min_element(mid + 1, tend, comp));
        return {std::move(*mid), std::move(*(mid + 1))};
    }
}

template <typename RandomIt>
auto median(RandomIt begin, RandomIt end)
        -> median_result<
            std::remove_cv_t<std::remove_reference_t<decltype(*begin)>>> {
    return median(begin, end, std::less<>{});
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
