#include <algorithm>
#include <iostream>
#include <map>
#include <string>

struct Display {
    std::string input[10];
    std::string output[4];
};

Display readLine() {
    Display d;
    for (int i = 0; i < 10; ++i)
        std::cin >> d.input[i];
    char sep;
    std::cin >> sep;
    for (int i = 0; i < 4; ++i)
        std::cin >> d.output[i];
    std::cin.ignore(16, '\n');
    return d;
}

std::map<char, char> findMapping(std::string (&input)[10]) {
    // sort all displays by length (number of segments on)
    // after that we are sure that the displays are ordered:
    // 1 7 4 x x x x x x 8
    std::sort(std::begin(input), std::end(input), [](auto&& s1, auto&& s2) {
        return s1.size() < s2.size();
    });
    std::map<char, char> mapping;

    // count occurences of each, distinguish both of the lines of 1
    std::map<char, int> occ;
    for (auto&& s : input)
        for (char c : s)
            ++occ[c];

    constexpr auto npos = std::string::npos;

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

int calculateOutput(std::string (&output)[4],
                    const std::map<char, char>& mapping) {
    for (auto& s : output) {
        for (auto& c : s)
            c = mapping.at(c);
        std::sort(std::begin(s), std::end(s));
    }

    const static std::map<std::string, int> disp_to_digit{
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
    int result = 0;

    constexpr int eof = std::istream::traits_type::eof();
    while (std::cin.peek() != eof && std::cin) {
        Display d = readLine();
        auto mapping = findMapping(d.input);
        result += calculateOutput(d.output, mapping);
    }

    std::cout << result;
}
