#pragma once

#include <algorithm>
#include <cassert>
#include <cctype>
#include <charconv>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
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

using llong = long long;
using ullong = unsigned long long;

template <typename T>
using vec = std::vector<T>;

using std::vector;
using std::string;
using std::set;
using std::map;
using std::queue;
using std::priority_queue;
using std::cin;
using std::cout;
using std::endl;
using std::swap;
using std::max;
using std::min;

template<typename T>
using limits = std::numeric_limits<T>;

namespace stdv = std::views;
namespace stdr = std::ranges;

namespace utils {
template<typename Range, typename Proj = std::identity>
void print(Range&& r, std::string_view sep = " ", Proj proj = {}) {
    std::cout << proj(*r.begin());
    std::ranges::for_each(r | std::views::drop(1), [sep, &proj](auto&& el) {
        std::cout << sep << proj(el); });
    std::cout << std::endl << std::endl;
}

template <typename T = int>
struct Point {
    T x{}, y{};
    auto operator<=>(const Point& other) const = default;
};

template<typename T>
struct Board {
    std::vector<std::vector<T>> fields;
    size_t width;
    size_t height;

    Board(size_t width, size_t height, T fill = {})
        : fields(height, std::vector<T>(width, fill)),
        width(width), height(height) { }

    struct Field {
        std::reference_wrapper<T> v;
        size_t row, col;

        Point<int> to_point() const { return { static_cast<int>(col),
                                               static_cast<int>(row) }; }
    };

    Field operator()(size_t row, size_t col) {
        return get(row, col);
    }

    Field get(size_t row, size_t col) {
        return Field{ fields[row][col], row, col };
    }
    Field get(Point<int> point) {
        return get(point.y, point.x);
    }

    T& getv(size_t row, size_t col) {
        return fields[row][col];
    }

    T& getv(Point<int> point) {
        return getv(point.y, point.x);
    }

    std::vector<Field> adjacent(size_t row, size_t col) {
        std::vector<Field> result;
        if (row != 0)
            result.push_back(get(row - 1, col));
        if (row != height - 1)
            result.push_back(get(row + 1, col));
        if (col != 0)
            result.push_back(get(row, col - 1));
        if (col != width - 1)
            result.push_back(get(row, col + 1));
        return result;
    }

    std::vector<Field> adjacent(Field f) { return adjacent(f.row, f.col); }

    std::vector<Field> adjacent_corner(size_t row, size_t col) {
        std::vector<Field> result;
        if (row != 0) {
            if (col != 0)           result.push_back(get(row - 1, col - 1));
            result.push_back(get(row - 1, col));
            if (col != width - 1)   result.push_back(get(row - 1, col + 1));
        }
        {
            if (col != 0)           result.push_back(get(row, col - 1));
            if (col != width - 1)   result.push_back(get(row, col + 1));
        }
        if (row != height - 1) {
            if (col != 0)           result.push_back(get(row + 1, col - 1));
            result.push_back(get(row + 1, col));
            if (col != width - 1)   result.push_back(get(row + 1, col + 1));
        }
        return result;
    }
    
    std::vector<Field> adjacent_corner(Field f) { return adjacent_corner(f.row, f.col); }

    std::vector<Field> all() {
        std::vector<Field> result;
        for (size_t row = 0; row < height; ++row) {
            for (size_t col = 0; col < width; ++col) {
                result.push_back(get(row, col));
            }
        }
        return result;
    }

    Board framed(T frame_fill) {
        Board<T> result = *this;
        result.height += 2;
        result.width += 2;

        for (auto& row : result.fields) {
            row.insert(row.begin(), frame_fill);
            row.push_back(frame_fill);
        }

        std::vector<T> empty_row(result.width, frame_fill);
        result.fields.insert(result.fields.begin(), empty_row);
        result.fields.push_back(empty_row);

        return result;
    }

    template <typename Proj = std::identity,
              typename U = std::invoke_result_t<Proj, T>>
    Board<U> projected(Proj proj = {}) const {
        Board<U> b(width, height);
        std::ranges::transform(fields, b.fields.begin(), [&proj](auto& row) {
            std::vector<U> new_row(row.size());
            std::ranges::transform(row, new_row.begin(), proj);
            return new_row;
            });
        return b;
    }

    void print(std::string_view sep = " ", int width_per_element = 0) {
        for (const auto& row : fields) {
            for (const auto& element : row) {
                if (width_per_element > 0)
                    std::cout << std::setw(width_per_element);
                std::cout << element;
                std::cout << sep;
            }
            std::cout << std::endl;
        }
    }

    struct EmptyState {};

    template<typename EnterCond, typename Transform, typename State = EmptyState>
    void flood_fill(EnterCond enter_cond, Transform transform, State&& state = {}) {
        auto visit = [&](auto& self, int row, int col) ->void {
            auto current = get(row, col);
            if (!enter_cond(*this, current)) return;
            if constexpr (std::is_same_v<State, EmptyState>)
                transform(*this, current);
            else
                transform(*this, current, state);
            std::vector neighbours = this->adjacent(row, col);
            for (const auto& n : neighbours)
                self(self, n.row, n.col);
        };
        for (auto&& f : all()) visit(visit, f.row, f.col);
    }

    template<typename EnterCond, typename Transform, typename State = EmptyState>
    void flood_fill_corners(EnterCond enter_cond, Transform transform, State&& state = {}) {
        auto visit = [&](auto& self, int row, int col) ->void {
            auto current = get(row, col);
            if (!enter_cond(*this, current)) return;
            if constexpr (std::is_same_v<State, EmptyState>)
                transform(*this, current);
            else
                transform(*this, current, state);
            std::vector neighbours = this->adjacentCorner(row, col);
            for (const auto& n : neighbours)
                self(self, n.row, n.col);
        };
        for (auto&& f : all()) visit(visit, f.row, f.col);
    }
};

template<typename T>
using BoardField = Board<T>::Field;

template<typename T>
Board<T> read_board() {
    std::string line;
    std::vector<std::vector<T>> preboard;
    while (std::getline(std::cin, line) && !line.empty()) {
        std::stringstream ss{ line };
        std::vector<T> row;
        T next;
        while (ss >> next) row.push_back(next);
        preboard.push_back(row);
    }

    int w = preboard[0].size();
    int h = preboard.size();

    Board<T> b(0, 0);
    b.width = w;
    b.height = h;
    b.fields = std::move(preboard);
    return b;
}


struct ScanResult {
    using Iss = std::istringstream;
    using Ss = std::stringstream;
    ScanResult(size_t scanned, bool success, std::istream& str)
        : scanned(scanned), success(success), remain(str) {}
    ScanResult(size_t scanned, bool success, Iss&& iss)
        : scanned(scanned), success(success),
        attached(std::make_unique<Iss>(std::move(iss))),
        remain(*this->attached.get()) {}
    ScanResult(size_t scanned, bool success, Ss&& ss)
        : scanned(scanned), success(success),
        attached(std::make_unique<Iss>(ss.str().substr(ss.tellg()))),
        remain(*this->attached.get()) {}

    operator bool() const { return success; }

    size_t scanned;
    bool success;
    std::unique_ptr<std::istringstream> attached;
    std::istream& remain;
};

template<typename T>
ScanResult scan_vector(std::vector<T>& out, std::istream& in,
                       std::string_view separator = " ", char delimiter = 0) {
    out.clear();
    T next;
    size_t initial_size = out.size(); 
    while (in.peek() != EOF && in.peek() != delimiter && in >> std::ws &&
           in.peek() != EOF && in.peek() != delimiter && in >> next) {
        out.push_back(next);
        if (separator != " ") {
            size_t eq = 0;
            for (; eq < separator.size(); eq++) {
                if (in.peek() != separator[eq]) break;
                in.ignore();
            }
            if (eq != separator.size()) {
                while (eq--) in.unget(); // impl defined, let's hope it works
                break; 
            }
        }
    }

    if (in.eof()) in.clear(), in.ignore(); // clear error, leave eof

    return ScanResult{initial_size != out.size(), initial_size != out.size(), in};
};

template<typename T>
ScanResult scan_vector(std::vector<T>& out, std::string_view text, std::string_view separator = " ", char delimiter = 0) {
    std::istringstream is(std::string{text});
    auto result = scan_vector(out, is, separator, delimiter);
    return ScanResult{result.scanned, result.success, is};
};

template<typename T>
ScanResult input_vector(std::vector<T>& out, std::string_view separator = " ", char delimiter = 0) {
    return scan_vector(out, std::cin, separator, delimiter);
};

template<typename T>
ScanResult input_line_vector(std::vector<T>& out, std::string_view separator = " ", char delimiter = 0) {
    std::string line;
    std::getline(std::cin, line);
    if (!std::cin) return ScanResult{0, false, std::cin};
    return scan_vector(out, line, separator, delimiter);
};


namespace detail {
constexpr char eof = std::istream::traits_type::eof();

template<typename Test, template<typename...> class Ref>
struct is_specialization : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

template<typename Test, template<typename...> class Ref>
inline constexpr bool is_specialization_v = is_specialization<Test, Ref>::value;

template<typename T>
using is_std_vector = is_specialization<T, std::vector>;

template<typename T>
inline constexpr bool is_std_vector_v = is_std_vector<T>::value;

template<typename T>
using is_std_pair = is_specialization<T, std::pair>;

template<typename T>
inline constexpr bool is_std_pair_v = is_std_pair<T>::value;

inline void scan_delimiters(std::istream& in, std::string_view& format) {
    const char* format_ptr = format.data();
    size_t size = format.size();
    while (size > 0 && in.good()) {
        char next = format_ptr[0];
        if (next == '{') {
            if (size == 1) throw;
            if (format_ptr[1] != '{') {
                break;
            }
            // skip first {
            ++format_ptr; --size;
        }
        // skip the delimiter character
        ++format_ptr; --size;
        if (next == ' ') {
            // skip whitespace
            char nextnext = size == 1 ? '\0' : format_ptr[1];
            char peeked = in.peek();
            while (peeked != eof  &&
                    peeked != nextnext &&
                    std::isspace(peeked)) {
                in.ignore();
                peeked = in.peek();
            }
            if (peeked == eof) break;
        }
        else {
            char peeked = in.peek();
            if (peeked != next) break;
            in.ignore();
        }
    }
    format = std::string_view{format_ptr, size};
}

template<typename T, typename... Args>
ScanResult scan_rec(std::istream& in, std::string_view format, T& arg1, Args&... args) {
    if (format.front() != '{') {
        return ScanResult{0, false, in};
    }
    const char* format_ptr = format.data();
    size_t size = format.size();

    ++format_ptr;
    --size;
    if (size == 0) throw;

    if constexpr (std::is_integral_v<T>) {
        switch (format_ptr[0]) {
            case 'x':
                ++format_ptr;
                --size;
                in >> std::hex;
                break;
            case 'o':
                ++format_ptr;
                --size;
                in >> std::oct;
                break;
            case 'd':
                ++format_ptr;
                --size;
                in >> std::dec;
                break;
            default:
                in >> std::dec;
        }
        in >> arg1;
        in >> std::dec;
    } else if constexpr (is_std_vector_v<T>) {
        const char* vec_format_ptr = format_ptr;
        size_t vec_format_size = 0;
        while (vec_format_size < size && vec_format_ptr[vec_format_size] != '}')
            ++vec_format_size;
        std::string_view separator =
            vec_format_size == 0
                ? std::string_view{" "}
                : std::string_view{vec_format_ptr, vec_format_size};
        char delimiter = 
            size <= vec_format_size + 1 ? '\0' : format_ptr[vec_format_size + 1];
        delimiter = delimiter == separator[0] || delimiter == '{' ? '\0' : delimiter;
        scan_vector<typename T::value_type>(arg1, in, separator, delimiter);
        format_ptr += vec_format_size;
        size -= vec_format_size;
    } else {
        in >> arg1;
    }

    if (size == 0 || format_ptr[0] != '}' || !in) {
        return ScanResult{0, false, in};
    }

    ++format_ptr;
    --size;

    format = std::string_view{format_ptr, size};

    detail::scan_delimiters(in, format);
    if constexpr(sizeof...(Args) == 0) {
        if (format.empty()) {
            return ScanResult{1, true, in};
        }
        else {
            return ScanResult{1, false, in};
        }
    } else {
         auto result = detail::scan_rec(in, format, args...);
        ++result.scanned;
        return result;
    }
}
}

template<typename... Args>
ScanResult scan(std::istream& in, std::string_view format, Args&... args) {
    // First, read all the trailing
    detail::scan_delimiters(in, format);
    if constexpr(sizeof...(Args) == 0) {
        if (format.empty()) {
            return ScanResult{0, true, in};
        } else {
            return ScanResult{0, false, in};
        }
    } else {
        return detail::scan_rec(in, format, args...);
    }
}

template<typename... Args>
ScanResult scan(std::string_view text, std::string_view format, Args&... args) {
    std::istringstream in(std::string{text});
    auto result = scan(in, format, args...);
    return ScanResult{result.scanned, result.success, in};
}

template<typename... Args>
ScanResult input(std::string_view format, Args&... args) {
    return scan(std::cin, format, args...);
}

template<typename... Args>
ScanResult input_line(std::string_view format, Args&... args) {
    std::string line;
    std::getline(std::cin, line);
    return scan(line, format, args...);
}

template<typename Range, typename Acc = std::ranges::range_value_t<Range>,
         typename Proj = std::identity>
auto sum(Range&& r, Acc zero = {}, Proj proj = {}) {
    return std::accumulate(std::ranges::begin(r), std::ranges::end(r), zero,
        [=](Acc acc, auto&& element) -> Acc {
            return acc + std::invoke(proj, element);
        });
}

template<typename Range, typename Acc = std::ranges::range_value_t<Range>,
         typename Proj = std::identity>
auto product(Range&& r, Acc one = {1}, Proj proj = {}) {
    return std::accumulate(std::ranges::begin(r), std::ranges::end(r), one,
        [=](Acc acc, auto&& element) -> Acc {
            return acc * std::invoke(proj, element);
        });
}

template<typename T>
size_t to_chars_len(T value, uint base = 10) noexcept
{
    uint n = 1;
    for (;;)
	{
	  if (value < base) return n;
	  value /= base;
	  n++;
	}
}

template<typename T>
std::string int_to_string(T value, size_t base) {
    static_assert(std::is_integral_v<T>);
    bool neg = value < 0;
    using UT = std::make_unsigned_t<T>;
    UT absvalue = neg ? -static_cast<UT>(value) : static_cast<UT>(value);
    size_t len = to_chars_len(absvalue, base);
    std::string result(neg + len, '\0');
    if (auto [_, ec] = std::to_chars(result.data(), result.data() + len, value);
        ec != std::errc{}) throw;
    return result;
}

template <typename T, typename Unionizer = std::plus<>>
struct FindUnion {
    std::vector<T> elements;
    std::vector<size_t> reps;
    Unionizer unionizer;

    FindUnion(size_t count, T fill = {}, Unionizer u = {})
        : elements(count, fill), reps(count), unionizer{u} {
        for(size_t i = 0; i < count; ++i) {
            reps[i] = i;
        }
    }

    size_t size() const { return reps.size(); }

    struct Group {
        size_t rep;
        T value;
    };

    std::vector<Group> get_groups() {
        std::vector<Group> result;
        for (size_t i = 0; i < size(); ++i)
            if (reps[i] == i)
                result.emplace_back(i, elements[i]);
        return result;
    }

    size_t find(size_t id) {
        size_t rep = id;
        while (rep != reps[rep]) rep = reps[rep];
        while (id != rep) {
            size_t idnext = reps[id];
            reps[id] = rep;
            id = idnext;
        }
        return rep;
    }

    void union_(size_t id1, size_t id2) {
        size_t rep1 = find(id1), rep2 = find(id2);
        if (rep1 == rep2) return;
        reps[rep2] = rep1;
        elements[rep1] = unionizer(elements[rep1], elements[rep2]);
    }
};

inline std::string to_upper(const std::string& str) {
    std::string result = str;
    for (char& c : result)
        c = toupper(static_cast<unsigned char>(c));
    return result;
}

inline std::string to_lower(const std::string& str) {
    std::string result = str;
    for (char& c : result)
        c = tolower(static_cast<unsigned char>(c));
    return result;
}

template <typename T>
using pair_of = std::pair<T, T>;

std::vector<std::string> split_text(std::string text, char sep = ' ') {
    size_t pos = text.find(sep);
    size_t initial_pos = 0;
    std::vector<std::string> result;

    while (pos != std::string::npos) {
        result.emplace_back(text.substr(initial_pos, pos - initial_pos));
        initial_pos = pos + 1;
        pos = text.find(sep, initial_pos);
    }

    // Add the last one
    result.push_back(text.substr(initial_pos));

    return result;
}

std::pair<std::string, std::string> split_text_at_first(std::string text, char sep = ' ') {
    size_t pos = text.find(sep);
    return std::make_pair(
        text.substr(0, pos),
        pos == std::string::npos ? std::string{} : text.substr(pos + 1));
}


struct HashPair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        using std::hash;
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return (17 * 31 + hash1) * 31 + hash2;
    }
};

template <
    typename T,
    typename H = std::conditional_t<
        utils::detail::is_std_pair_v<T>,
        HashPair,
        std::hash<T>>>
    using hashset = std::unordered_set<T, H>;
template <
    typename T,
    typename U,
    typename H = std::conditional_t<
        utils::detail::is_std_pair_v<T>,
        HashPair,
        std::hash<T>>>
    using hashmap = std::unordered_map<T, U, H>;

template <typename T>
class median_result {
    public:
    inline bool empty() const {
        return state_ == state::empty;
    }

    inline bool distinct() const {
        return state_ == state::one_value;
    }

    inline operator T() const {
        return empty() ? T{}
                : distinct() ? low_
                : std::midpoint(low_, high_);
    }

    inline T low() const {
        return low_;
    }

    inline T high() const {
        return high_;
    }

    template <typename RandomIt, typename Compare>
    requires std::random_access_iterator<RandomIt>
    friend auto median(RandomIt begin, RandomIt end, Compare comp)
        -> median_result<
            std::remove_cv_t<std::remove_reference_t<decltype(*begin)>>>;

    private:
    median_result() : state_{state::empty} {}
    median_result(T&& low)
        : state_{state::one_value}, low_{std::move(low)} {}
    median_result(T&& low, T&& high)
        : state_{state::two_values}, low_{std::move(low)}, high_{std::move(high)} {}

    enum class state : char {
        empty,
        one_value,
        two_values
    } state_;
    T low_, high_;
};

template <typename RandomIt, typename Compare = std::ranges::less>
requires std::random_access_iterator<RandomIt>
auto median(RandomIt begin, RandomIt end, Compare comp = {})
        -> median_result<
            std::remove_cv_t<std::remove_reference_t<decltype(*begin)>>> {
    auto size = end - begin;
    if (size == 0) {
        return {};
    }

    std::vector temp_buffer(begin, end);
    auto tbegin = temp_buffer.begin();
    auto tend = temp_buffer.end();
    if (size % 2 == 1) {
        auto mid = tbegin + size / 2;
        std::nth_element(tbegin, mid, tend, comp);
        return {std::move(*mid)};
    }
    else {
        auto mid = tbegin + size / 2 - 1;
        std::nth_element(tbegin, mid, tend, comp);
        std::iter_swap(
            mid + 1,
            std::min_element(mid + 1, tend, comp));
        return {std::move(*mid), std::move(*(mid + 1))};
    }
}

template <typename Range, typename Compare = std::ranges::less>
requires std::ranges::common_range<Range> 
        && std::ranges::random_access_range<Range>
auto median(Range range, Compare comp = {})
        -> median_result<
            std::remove_cv_t<std::remove_reference_t<decltype(*std::ranges::begin(range))>>> {
    return median(std::ranges::begin(range), std::ranges::end(range), comp);
}

template <typename Range, typename T>
requires std::ranges::random_access_range<Range>
bool find_pos(Range&& range, const T& element) {
    return std::ranges::find(range, element) - std::ranges::begin(range);
}

template <typename Range, typename T>
requires std::ranges::input_range<Range>
bool contains(Range&& range, const T& element) {
    return std::ranges::find(range, element) == std::ranges::end(range);
}

template <typename T, typename Comp>
struct compares_to {
    T v;
    Comp comp = {};
    template <typename U>
    bool operator()(const U& other) { return comp(other, v); }
};

template <typename T>
using less_than = compares_to<T, stdr::less>;
template <typename T>
using lesseq_than = compares_to<T, stdr::less_equal>;
template <typename T>
using greater_than = compares_to<T, stdr::greater>;
template <typename T>
using greatereq_than = compares_to<T, stdr::greater_equal>;
template <typename T>
using eq_to = compares_to<T, stdr::equal_to>;

namespace detail {
struct pair_first_impl {
    template <typename T, typename U>
    auto&& operator()(const std::pair<T, U>& p) {
        return (p.first);
    }
    template <typename T, typename U>
    auto&& operator()(std::pair<T, U>&& p) {
        return std::move(p.first);
    }
};

struct pair_second_impl {
    template <typename T, typename U>
    auto&& operator()(const std::pair<T, U>& p) {
        return (p.second);
    }
    template <typename T, typename U>
    auto&& operator()(std::pair<T, U>&& p) {
        return std::move(p.second);
    }
};
}

inline namespace CPOs {
    inline constexpr detail::pair_first_impl pair_first;
    inline constexpr detail::pair_second_impl pair_second;
}

}

using utils::hashmap;
using utils::hashset;
