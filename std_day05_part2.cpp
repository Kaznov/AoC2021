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
        int xdiff = x1 - x0;
        int ydiff = y1 - y0;
        auto abs = [](int x) -> int { return x >= 0 ? x : -x; };

        if (xdiff != 0 && ydiff != 0 && abs(xdiff) != abs(ydiff))
            continue;

        int xstep = xdiff == 0 ? 0 : xdiff / abs(xdiff);
        int ystep = ydiff == 0 ? 0 : ydiff / abs(ydiff);

        int steps = std::max(abs(xdiff), abs(ydiff));

        for (int i = 0, x = x0, y = y0; i <= steps; ++i, x += xstep, y += ystep)
            ++vents_locations[std::make_pair(x, y)];
    }

    size_t overlaping_vents = std::count_if(std::begin(vents_locations),
                                            std::end(vents_locations),
                                            [](auto&& p) -> bool {
                                                return p.second > 1;
                                            });
    printf("Part2: %zu\n", overlaping_vents);
}
