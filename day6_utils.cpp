#include "utils.hh"

// PART 1:
// constexpr int kDays = 80;
// PART 2:
constexpr int kDays = 256;

int main() {
    ulong fishes_of_epoch[9] = {0};
    int next_fish;

    while (std::cin >> next_fish)
        ++fishes_of_epoch[next_fish];

    for (int day = 0; day < kDays; ++day) {
        // That's a rotate!
        std::rotate(fishes_of_epoch, fishes_of_epoch + 1, fishes_of_epoch + 9);
        fishes_of_epoch[6] += fishes_of_epoch[8];
    }

    auto sum = utils::sum(fishes_of_epoch);
    std::cout << sum;
}
