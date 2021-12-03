#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using uint = unsigned;
constexpr int kBinLen = 5; // length of binary numbers

auto bin_to_num(const std::string& text_bin) -> uint {
    unsigned int num = 0;
    for (int i = 0; i < kBinLen; ++i)
        num = (num << 1) | text_bin[i] == '1';
    return num;
};

template <typename SizeSelector>
auto ratingFinder(const std::vector<uint>& input,
                  SizeSelector&& selector) -> uint {
    // assert (is_sorted)
    auto it_lower = std::begin(input), it_upper = std::end(input);
    for (int i = 0; i < kBinLen && it_upper - it_lower > 1; ++i) {
        uint set_bits = ((*it_lower) >> (kBinLen - i))
                        << (kBinLen - i);
        uint separator = set_bits | 1 << (kBinLen - 1 - i);
        auto it_mid = std::lower_bound(it_lower, it_upper, separator);
        auto zeros_count = it_mid - it_lower;
        auto ones_count = it_upper - it_mid;
        (selector(zeros_count, ones_count) ?
            it_lower : it_upper) = it_mid;
    }

    return *it_lower;
}

int main() {
    std::vector<std::string> input;
    std::copy(std::istream_iterator<std::string>{std::cin},
              std::istream_iterator<std::string>{},
              std::back_inserter(input));

    std::vector<uint> input_nums;
    std::transform(std::begin(input), std::end(input),
                   std::back_inserter(input_nums), bin_to_num);

    std::sort(std::begin(input_nums), std::end(input_nums));

    std::sort(std::begin(input), std::end(input));
    for (auto n : input) std::cout << n << "\n";

    unsigned oxy_rating = ratingFinder(input_nums, std::less_equal<>{});
    unsigned co2_rating = ratingFinder(input_nums, std::greater<>{});

    std::cout << oxy_rating * co2_rating;
}
