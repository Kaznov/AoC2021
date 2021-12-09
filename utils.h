#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;
using i64 = int64_t;
using i32 = int32_t;
using i16 = int16_t;
using i8 = int8_t;

using uint = unsigned;
using ulong = unsigned long;

namespace utils{
template<typename Range, typename Proj = std::identity>
void print(Range&& r, std::string_view sep = " ", Proj proj = {}) {
    std::cout << proj(*r.begin());
    std::ranges::for_each(r | std::views::drop(1), [sep, &proj](auto&& el) {
        std::cout << sep << proj(el); });
}

struct ScanResult {
    size_t scanned;
    bool success;
    std::istream remain;
};

template<typename... Args>
ScanResult scan(std::istream& in, std::string_view format, Args&... args) {
    auto result = scan(in, format, args...);
    ++result.scanned;
    return result;
}

template<typename... Args>
ScanResult scan(std::string_view text, std::string_view format, Args&... args) {
    std::istringstream in(std::string{text});
    return scan_rec(in, format, args...);
}

template<typename... Args>
ScanResult input(std::string_view format, Args&... args) {
    return scan_rec(std::cin, format, args...);
}

template<typename... Args>
ScanResult input_line(std::string_view text, std::string_view format, Args&... args) {
    std::string line;
    std::getline(std::cin, line);
    return scan(std::istringstream(std::move(line)), format, args...);
}

template<typename T>
ScanResult scan_vector(std::vector<T>& out, std::istream& in, std::string_view separator = " ") {};

template<typename T>
ScanResult scan_vector(std::vector<T>& out, std::string_view text, std::string_view separator = " ") {};

template<typename T>
ScanResult input_vector(std::vector<T>& out, std::string_view separator = " ") {};


template<typename T>
std::istream& operator>>(std::istream& in, std::vector<T>& out) {

}

template<typename Range>
auto sum(Range&& r, auto zero = std::ranges::range_value_t<Range>{}) {
    return std::accumulate(std::ranges::begin(r), std::ranges::end(r), zero);
}


}
