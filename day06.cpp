#include "utils.hh"

constexpr int kDaysPart1 = 80;
constexpr int kDaysPart2 = 256;

int main() {
    ulong fishes_of_epoch[9] = {0};
    int next_fish;

    while (std::cin >> next_fish)
        ++fishes_of_epoch[next_fish];

    for (int day = 0; day < kDaysPart1; ++day) {
        // That's a rotate!
        std::rotate(fishes_of_epoch, fishes_of_epoch + 1, fishes_of_epoch + 9);
        fishes_of_epoch[6] += fishes_of_epoch[8];
    }

    std::cout << "Part 1: " << utils::sum(fishes_of_epoch) << "\n";

    for (int day = kDaysPart1; day < kDaysPart2; ++day) {
        // That's a rotate!
        std::rotate(fishes_of_epoch, fishes_of_epoch + 1, fishes_of_epoch + 9);
        fishes_of_epoch[6] += fishes_of_epoch[8];
    }

    std::cout << "Part 2: " << utils::sum(fishes_of_epoch) << "\n";
}
