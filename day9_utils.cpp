#include "utils.hh"

int main() {
    auto board = utils::read_board<char>();

    int w = board.width;
    int h = board.height;

    auto level = [](const auto& f) -> int { return f.v - '0' + 1; };
    auto id = [=](const auto& f) -> int { return f.row * w + f.col; };
    
    int low_point_sum = 0;
    utils::FindUnion<int> fu(w * h, 1);

    for (auto& field : board.all()) {
        int field_level = level(field);
        auto adjacent = board.adjacent(field);
        low_point_sum += 
            field_level * stdr::all_of(adjacent,
                                       utils::greater_than<int>(field_level),
                                       level);
        for (auto& adj: adjacent)
            if (field_level < level(adj) && level(adj) < 10)
                fu.union_(id(field), id(adj));
    }

    auto regions = fu.get_groups();
    stdr::partial_sort(regions, regions.begin() + 3,
                       stdr::greater{}, &utils::FindUnion<int>::Group::value);

    std::cout << "Part 1: " << low_point_sum << "\n"
              << "Part 2: " << regions[0].value
                             * regions[1].value
                             * regions[2].value << "\n";
}
