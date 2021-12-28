#include <algorithm>
#include <cstdio>
#include <numeric>

constexpr int kDays = 256;

int main() {
    unsigned long long fishes_of_epoch[9] = {0};
    int next_fish;

    while (scanf("%d,", &next_fish) == 1)
        ++fishes_of_epoch[next_fish];

    for (int day = 0; day < kDays; ++day) {
        // That's a rotate!
        std::rotate(fishes_of_epoch, fishes_of_epoch + 1, fishes_of_epoch + 9);
        fishes_of_epoch[6] += fishes_of_epoch[8];
    }

    auto sum = std::accumulate(fishes_of_epoch, fishes_of_epoch + 9, 0ull);
    printf("Part 2: %llu\n", sum);
}
