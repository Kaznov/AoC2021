#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace stdr = std::ranges;
using uint = unsigned;
constexpr int kBinLen = 12; // length of binary numbers

auto ratingFinder(const std::vector<uint>& input,
                  bool is_co2) -> uint {
    // assert (is_sorted)
    // perform binary-like (radix) search on the input vector
    auto it_lower = std::begin(input), it_upper = std::end(input);
    for (int i = 0; i < kBinLen && it_upper - it_lower > 1; ++i) {
        // higher i from 12 bits are already known, set them
        uint set_bits = ((*it_lower) >> (kBinLen - i))
                        << (kBinLen - i);
        // to find, if there is more 1s or 0,
        // find partition point at [set_bits][1][000...0]
        uint separator = set_bits | 1 << (kBinLen - 1 - i);
        // partition_pount would be more idiomatic,
        // but a bit slower and more cryptic
        auto it_mid = std::lower_bound(it_lower, it_upper, separator);
        auto zeros_count = it_mid - it_lower;
        auto ones_count = it_upper - it_mid;
        // depending if we look for most zeroes or ones,
        // update either lower or upper bound of searched range
        ((zeros_count <= ones_count) ^ is_co2 ?
            it_lower : it_upper) = it_mid;
    }

    return *it_lower;
}

int main() {
    using string_it = std::istream_iterator<std::string>;
    std::vector<std::string> input(string_it{std::cin}, string_it{});

    std::vector<uint> input_nums;
    auto bin_to_num = [](const std::string& text_bin) -> uint {
        return static_cast<uint>(std::stoul(text_bin, nullptr, 2));
    };

    stdr::transform(input, std::back_inserter(input_nums), bin_to_num);
    stdr::sort(input_nums);

    uint oxy_rating = ratingFinder(input_nums, false);
    uint co2_rating = ratingFinder(input_nums, true);

    std::cout << oxy_rating * co2_rating;
}
