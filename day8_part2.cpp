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
    // sort each display's segments alphabetically
    for (auto& in : input) std::sort(std::begin(in), std::end(in));

    // mapping [segment in new] -> [segment in original]
    std::map<char, char> mapping;

    {
        // first, find what 'a' (top segment) is mapped to
        // - set difference between display of 7 and 1
        char c;
        std::ranges::set_difference(input[1], input[0], &c);
        mapping[c] = 'a';
    }
    {
        // count occurences of each segment
        // expected occurences: a: 8, b: 6, c: 8, d: 7, e: 4, f: 9, g: 7
        std::map<char, int> occ;
        for (auto&& s : input)
            for (char c : s)
                ++occ[c];

        for (auto&[c, count] : occ) {
            if (count == 9) mapping[c] = 'f';
            if (count == 6) mapping[c] = 'b';
            if (count == 4) mapping[c] = 'e';
            // both 'a' and 'c' should occur 8 times,
            // but 'a' was already mapped
            if (count == 8 && !mapping.contains(c))
                mapping[c] = 'c';
        }
    }

    // only 'd' and 'g' left - 'd' is lit on in display of 4
    for (char c = 'a'; c <= 'g'; ++c) {
        if (mapping.contains(c)) continue;
        if (input[2].contains(c)) mapping[c] = 'd';
        else mapping[c] = 'g';
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

    while (std::cin.peek() && std::cin) {
        Display d = readLine();
        auto mapping = findMapping(d.input);
        result += calculateOutput(d.output, mapping);
    }

    std::cout << result;
}
