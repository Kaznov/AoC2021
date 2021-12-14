#include <cmath>
#include <cstdio>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> crabs;
    int next_crab;

    while (scanf("%d,", &next_crab) == 1) {
        crabs.push_back(next_crab);
    }

    int sum = std::accumulate(std::begin(crabs), std::end(crabs), 0);
    int guess = std::round(static_cast<double>(sum) / crabs.size());

    printf("Initial guess: %d\n", guess);

    auto cost = [&](int target) -> long{
        return std::accumulate(
            std::begin(crabs), std::end(crabs), 0l,
            [=](long acc, int next) {
                long dist = std::abs(next - target);
                return acc + dist * (dist + 1) / 2; });
    };

    long best_cost = cost(guess);
    long next_cost;
    while ((next_cost = cost(guess - 1)) < best_cost) {
        --guess;
        best_cost = next_cost;
    }
    while ((next_cost = cost(guess + 1)) < best_cost) {
        ++guess;
        best_cost = next_cost;
    }

    printf("Target: %d\n", guess);
    printf("Cost: %ld", best_cost);
}
