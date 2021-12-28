#include "utils.hh"

constexpr i64 xinc[14] = {15, 10, 12, 10, 14, -11, 10, -16, -9, 11, -8, -8, -10, -9 };
constexpr i64 zdiv[14] = { 1,  1,  1,  1,  1,  26,  1,  26, 26,  1, 26, 26,  26, 26 };
constexpr i64 yinc[14] = {13, 16,  2,  8, 11,   6, 12,   2,  2, 15,  1, 10,  14, 10 };

i64 algo(i64 n) {
    auto get = [div=10000000000000, n]() mutable {
        int digit = (n/div) % 10;
        div /= 10;
        return digit;
    };

    i64 w = 0, z = 0;
    for (int i = 0; i < 14; i++) {
        w = get();
        if (w == 0) {
            z = limits<i64>::max(); break;
        }
        bool b = ((z % 26) + xinc[i]) != w;

        z /= zdiv[i];
        if (b) {
            z *= 26;
            z += w + yinc[i];
        }
    }

    return z;
}

template<typename Accumulator>
i64 improve(i64 n, Accumulator acc) {
    for (i64 i = 0, inc = 1; i < 14; ++i, inc *= 10)
    for (i64 j = i + 1, inc2 = inc * 10; j < 14; ++j, inc2 *= 10)
        while (algo(acc(n, inc + inc2)) == 0) {
            n = acc(n, inc + inc2);
            cout << n << "\n";
        }

    return n;
}

int main() {
    utils::Rng rng(std::random_device{}()); // 64bit state xorshift
    std::uniform_int_distribution<i64> dis(11'111'111'111'111,
                                           99'999'999'999'999);

    // First, find one that satisfies the algorithm
    i64 n;

    i64 best_algo_result = limits<i64>::max();
    do {
        n = dis(rng);
        i64 result = algo(n);
        if (result < best_algo_result) {
            best_algo_result = result;
            cout << "Best attempt: " << best_algo_result << "\n";
        }
    } while (best_algo_result != 0); // this might take a while


    cout << "Found number satisfying algo!\n" << n << "\n\n";

    cout << "Increasing:\n";
    auto max_solution = improve(n, std::plus<>{});
    cout << "Part 1: " << max_solution << "\n\n";
    cout << "Decreasing:\n";
    auto min_solution = improve(n, std::minus<>{});
    cout << "Part 2: " << min_solution << "\n";
}
