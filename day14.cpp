#include "utils.hh"

int main() {
    string pattern;
    getline(cin, pattern);
    cin.ignore(16,'\n'); // empty line

    map<utils::pair_of<char>, char> productions;
    for (char c1, c2, p; utils::input_line("{}{} -> {}", c1, c2, p);)
        productions.emplace(std::pair{ c1, c2 }, p);

    map<utils::pair_of<char>, llong> occurences;
    for (size_t i = 0; i < pattern.size() - 1; ++i)
        ++occurences[{pattern[i], pattern[i + 1]}];

    auto step = [&]() {
        map<utils::pair_of<char>, llong> new_occurences;
        for (auto&& [pair, count] : occurences) {
            auto product = productions.at(pair);
            new_occurences[{pair.first, product}] += count;
            new_occurences[{product, pair.second}] += count;
        }
        swap(new_occurences, occurences);
    };

    auto count_letters = [&]() -> map<char, llong> {
        map<char, llong> result;
        for (auto&& [pair, count] : occurences) {
            result[pair.first] += count;
            result[pair.second] += count;
        }
        ++result[pattern.front()];
        ++result[pattern.back()];
        for (auto& [_, count] : result)
            count /= 2;
        return result;
    };

    auto get_solution = [&]() -> llong {
        auto letters_count = count_letters();
        auto [min, max] = stdr::minmax_element(letters_count,
                                               stdr::less{},
                                               utils::pair_second);
        return max->second - min->second;
    };

    int steps_part1 = 10;
    for (int i = 0; i < steps_part1; ++i)
        step();
    cout << "Part 1: " << get_solution() << "\n";

    int steps_part2 = 40;
    for (int i = steps_part1; i < steps_part2; ++i)
        step();
    cout << "Part 2: " << get_solution() << "\n";
}
