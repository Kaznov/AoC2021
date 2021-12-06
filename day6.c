#include "stdio.h"

// PART 1:
// #define DAYS 80
// PART 2:
#define DAYS 256

int main() {
    unsigned long long fishes_of_epoch[9] = {0};
    int next_fish;

    while (scanf("%d,", &next_fish) == 1)
        ++fishes_of_epoch[next_fish];

    for (int day = 0; day < DAYS; ++day) {
        unsigned long long newborn = fishes_of_epoch[0];
        for (int e = 0; e < 8; ++e)
            fishes_of_epoch[e] = fishes_of_epoch[e + 1];
        fishes_of_epoch[8] = newborn;
        fishes_of_epoch[6] += newborn;
    }

    unsigned long long sum = 0;
    for (int e = 0; e < 9; ++e) sum += fishes_of_epoch[e];
    printf("%llu", sum);
}
