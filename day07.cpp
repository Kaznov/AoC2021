#include "utils.hh"

int main() {
    vec<int> crabs;
    utils::input_vector(crabs,",");

    int median_v = utils::median(crabs);
    int cost1 = std::accumulate(
        std::begin(crabs), std::end(crabs), 0,
        [=](int acc, int next) {
            return acc + abs(next - median_v);});

    cout << "Part 1: " << cost1 << "\n";

    int sum = utils::sum(crabs);
    int guess = std::round(static_cast<double>(sum) / crabs.size());

    auto cost2_fn = [&](int target) -> long {
        return std::accumulate(
            std::begin(crabs), std::end(crabs), 0l,
            [=](long acc, int next) {
                long dist = std::abs(next - target);
                return acc + dist * (dist + 1) / 2; });
    };

    long cost2 = cost2_fn(guess);
    long next_cost;
    while ((next_cost = cost2_fn(guess - 1)) < cost2) {
        --guess;
        cost2 = next_cost;
    }
    while ((next_cost = cost2_fn(guess + 1)) < cost2) {
        ++guess;
        cost2 = next_cost;
    }

    cout << "Part 2: " << cost2 << "\n";
}
