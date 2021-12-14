#include "utils.hh"

int main() {
    std::unordered_map<std::pair<int, int>, int, utils::HashPair> vents_locations1;
    std::unordered_map<std::pair<int, int>, int, utils::HashPair> vents_locations2;
    int x0, y0, x1, y1;
    while (utils::input_line("{},{} -> {},{}", x0, y0, x1, y1).success) {
        int xdiff = x1 - x0;
        int ydiff = y1 - y0;
        auto abs = [](int x) -> int { return x >= 0 ? x : -x; };

        if (xdiff != 0 && ydiff != 0 && abs(xdiff) != abs(ydiff))
            continue;

        bool diagonal = abs(xdiff) == abs(ydiff);
        int xstep = xdiff == 0 ? 0 : xdiff / abs(xdiff);
        int ystep = ydiff == 0 ? 0 : ydiff / abs(ydiff);

        int steps = std::max(abs(xdiff), abs(ydiff));

        for (int i = 0, x = x0, y = y0; i <= steps; ++i, x += xstep, y += ystep) {
            if (!diagonal)
                ++vents_locations1[std::make_pair(x, y)];
            ++vents_locations2[std::make_pair(x, y)];
        }
    }

    auto more_than_one = [](auto&& pos_count) { return pos_count.second > 1; };

    size_t overlaping_vents1 = 
        stdr::count_if(vents_locations1, more_than_one);
    size_t overlaping_vents2 = 
        stdr::count_if(vents_locations2, more_than_one);
                                            
    std::cout << "Part 1: " << overlaping_vents1 << "\n"
              << "Part 2: " << overlaping_vents2 << "\n";
}
