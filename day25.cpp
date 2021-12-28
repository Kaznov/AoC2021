#include "utils.hh"

int main() {
    auto board = utils::read_board<char>();
    bool change = true;
    int turn = 0;
    while (change) {
        ++turn;
        change = false;
        {
            utils::Board<char> copy(board.width, board.height, '.');
            for (const auto& f : board.all()) {
                if (f.v == 'v') {
                    copy.getv(f.row, f.col) = 'v';
                    continue;
                }
                if (f.v == '.') {
                    continue;
                }
                int next_col = (f.col + 1) % board.width;
                if (board.getv(f.row, next_col) == '.') {
                    copy.getv(f.row, next_col) = '>';
                    change = true;
                }
                else {
                    copy.getv(f.row, f.col) = '>';
                }
            }
            board = std::move(copy);
        }
        // board.print("");
        {
            utils::Board<char> copy(board.width, board.height, '.');
            for (const auto& f : board.all()) {
                if (f.v == '>') {
                    copy.getv(f.row, f.col) = '>';
                    continue;
                }
                if (f.v == '.') {
                    continue;
                }
                int next_row = (f.row + 1) % board.height;
                if (board.getv(next_row, f.col) == '.') {
                    copy.getv(next_row, f.col) = 'v';
                    change = true;
                }
                else {
                    copy.getv(f.row, f.col) = 'v';
                }
            }
            board = std::move(copy);
        }
        // board.print("");
    }

    std::cout << "Part 1: " << turn << "\n";
}
