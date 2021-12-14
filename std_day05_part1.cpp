#include <algorithm>
#include <cstdio>
#include <unordered_map>
#include <utility>

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        using std::hash;
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return (17 * 31 + hash1) * 31 + hash2;
    }
};

int main() {
    std::unordered_map<std::pair<int, int>, int, hash_pair> vents_locations;
    int x0, y0, x1, y1;
    while (scanf(" %d,%d -> %d,%d", &x0, &y0, &x1, &y1) == 4) {
        if (x0 != x1 && y0 != y1) continue;
        if (std::tie(x0, y0) > std::tie(x1, y1)) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        if (x0 == x1)
        for (int y = y0; y <= y1; ++y)
            ++vents_locations[std::make_pair(x0, y)];
        else
        for (int x = x0; x <= x1; ++x)
            ++vents_locations[std::make_pair(x, y0)];
    }

    size_t overlaping_vents = std::count_if(std::begin(vents_locations),
                                            std::end(vents_locations),
                                            [](auto&& p) -> bool {
                                                return p.second > 1;
                                            });
    printf("%zu", overlaping_vents);
}
