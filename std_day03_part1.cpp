#include <iostream>
#include <string>

int main() {
    std::string binary_num;
    constexpr int kBinaryLength = 12;
    int bits_occurences[kBinaryLength]{};

    while (std::cin >> binary_num) {
        for (int i = 0; i < kBinaryLength; ++i) {
            bits_occurences[i] += (binary_num[i] == '1' ? 1 : -1);
        }
    }

    unsigned gamma = 0;
    for (int i = 0; i < kBinaryLength; ++i) {
        gamma <<= 1;
        gamma |= bits_occurences[i] >= 0;
    }

    unsigned epsilon = (1 << kBinaryLength) - 1 - gamma;
    std::cout << "Part 1: " << gamma * epsilon << "\n";
}
