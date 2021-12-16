#include "utils.hh"

int floodBlink(utils::Board<char>& board, utils::BoardField<char> field) {
    field.v += field.v != '0';
    if (field.v <= '9')
        return 0;

    int blinks = 1;
    field.v.get() = '0';
    for (auto adj : board.adjacent_corner(field))
        blinks += floodBlink(board, adj);

    return blinks;
}

int main() {
    auto board = utils::read_board<char>();

    int turns = 0;
    int blinks = 0;
    while (true) {
        ++turns;
        for (auto& f : board.all())
                f.v += 1;
        
        for (auto& f : board.all())
            if (f.v > '9')
                blinks += floodBlink(board, f);

        if (turns == 100)
            std::cout << "Part 1: " << blinks << "\n";

        if (stdr::all_of(board.all(), [](auto& f) { return f.v == '0'; }))
            break;
    }

    std::cout << "Part 2: " << turns << "\n";
}
