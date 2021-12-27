#include "utils.hh"

int main() {
    string map;
    cin >> map;
    cin.ignore(16, '\n');
    cin.ignore(16, '\n');
    auto board = utils::read_board<char>();
    // board.print("");

    auto extend = [&](int turn) {
        char fill = (turn == 0
                     || map.front() == '.'
                     || turn % 2 == 0 && map.back() == '.') ? '.' : '#';

        board = board.framed(fill);
        board = board.framed(fill);
        // board.print("");

        auto copy = board;
        for (int row = 1; row < board.height - 1; ++row)
        for (int col = 1; col < board.width - 1; ++col) {
            u32 num = 0;
            auto square = board.square(row, col);
            for (auto&& f : square) {
                num <<= 1;
                num |= f.v == '#';
            }
            copy.getv(row, col) = map[num];
        }

        board = copy.unframed();
    };

    for (int i = 0; i < 2; ++i)
        extend(i);

    cout << "Part 1: "
         << stdr::count(board.all(), '#', &utils::BoardField<char>::v) << "\n";

    for (int i = 2; i < 50; ++i)
        extend(i);

     cout << "Part 2: "
         << stdr::count(board.all(), '#', &utils::BoardField<char>::v) << "\n";

}
