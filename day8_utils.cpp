#include "utils.hh"

std::map<char, char> findMapping(vec<string> input) {
    // sort all displays by length (number of segments on)
    // after that we are sure that the displays are ordered:
    // 1 7 4 x x x x x x 8
    stdr::sort(input, stdr::less{}, stdr::size);
    std::map<char, char> mapping;

    // count occurences of each, distinguish both of the lines of 1
    std::map<char, int> occ;
    for (auto&& s : input)
        for (char c : s)
            ++occ[c];

    constexpr auto npos = string::npos;

    // count occurences of each segment
    // expected occurences: a: 8, b: 6, c: 8, d: 7, e: 4, f: 9, g: 7
    for (auto&[c, count] : occ) {
        if (count == 4) mapping[c] = 'e';
        else if (count == 6) mapping[c] = 'b';
        else if (count == 9) mapping[c] = 'f';
        // 4 contains d (middle one) but not g (bottom one)
        else if (count == 7 && input[2].find(c) != npos) mapping[c] = 'd';
        else if (count == 7 && input[2].find(c) == npos) mapping[c] = 'g';
        // 1 contains c (top right) but not a (top)
        else if (count == 8 && input[0].find(c) != npos) mapping[c] = 'c';
        else if (count == 8 && input[0].find(c) == npos) mapping[c] = 'a';
    }

    return mapping;
}

int calculateOutput(vec<string> output,
                    const std::map<char, char>& mapping) {
    for (auto& s : output) {
        for (auto& c : s)
            c = mapping.at(c);
        stdr::sort(s);
    }

    const static std::map<string, int> disp_to_digit{
        {"abcefg", 0},
        {"cf", 1},
        {"acdeg", 2},
        {"acdfg", 3},
        {"bcdf", 4},
        {"abdfg", 5},
        {"abdefg", 6},
        {"acf", 7},
        {"abcdefg", 8},
        {"abcdfg", 9}
    };

    int result = 0;

    for(auto&& s : output) {
        result *= 10;
        result += disp_to_digit.at(s);
    }
    return result;
}

int main() {
    int result1 = 0;
    int result2 = 0;
    vec<string> input, output;
    while (utils::input_line("{}|{}", input, output).success) {
        assert(input.size() == 10);
        assert(output.size() == 4);
        auto mapping = findMapping(input);
        result1 += stdr::count_if(
            output,
            [](size_t s) { return s == 2 || s == 3 || s == 4 || s == 7; },
            stdr::size);
        result2 += calculateOutput(output, mapping);
    }

    std::cout << "Part 1: " << result1 << "\n";
    std::cout << "Part 2: " << result2 << "\n";
}
