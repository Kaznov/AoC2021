#include "utils.hh"

using uint = unsigned;
constexpr int kBinLen = 12; // length of binary numbers

auto ratingFinder(const vec<uint>& input,
                  bool is_co2) -> uint {
    // assert (is_sorted)
    auto it_lower = std::begin(input), it_upper = std::end(input);
    for (int i = 0; i < kBinLen && it_upper - it_lower > 1; ++i) {
        uint set_bits = ((*it_lower) >> (kBinLen - i))
                        << (kBinLen - i);
        uint separator = set_bits | 1 << (kBinLen - 1 - i);
        auto it_mid = std::lower_bound(it_lower, it_upper, separator);
        auto zeros_count = it_mid - it_lower;
        auto ones_count = it_upper - it_mid;
        ((zeros_count <= ones_count) ^ is_co2 ?
            it_lower : it_upper) = it_mid;
    }

    return *it_lower;
}

int main() {
    vec<string> input;
    utils::input_vector(input);

    int bits_occurences[kBinLen]{};
    for (const auto& binary_num : input)
    for (int i = 0; i < kBinLen; ++i)
        bits_occurences[i] += (binary_num[i] == '1' ? 1 : -1);
    
    // only bits that are more often 1
    unsigned int gamma = 0;
    for (int i = 0; i < kBinLen; ++i) {
        gamma <<= 1;
        gamma |= bits_occurences[i] >= 0;
    }
    
    // only bits that are more often 0
    unsigned int epsilon = ~gamma & ((1 << kBinLen) - 1);
    std::cout << "Part 1: " << epsilon * gamma << "\n";

    vec<uint> input_nums;
    auto bin_to_num = [](const auto& text_bin) -> uint {
        return std::stoi(text_bin, nullptr, 2);
    };
    stdr::transform(input,
                   std::back_inserter(input_nums), bin_to_num);

    stdr::sort(input_nums);

    unsigned oxy_rating = ratingFinder(input_nums, false);
    unsigned co2_rating = ratingFinder(input_nums, true);

    std::cout << "Part 2: " << oxy_rating * co2_rating << "\n";
}
