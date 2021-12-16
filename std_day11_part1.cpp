#include <iostream>
#include <ranges>
#include <string>
#include <vector>

int floodBlink(std::vector<std::string>& board, int row, int col) {
    if (row < 0 || col < 0
        || static_cast<size_t>(row) >= board.size()
        || static_cast<size_t>(col) >= board[0].size())
        return 0;

    board[row][col] += board[row][col] != '0';
    if (board[row][col] <= '9')
        return 0;

    int blinks = 1;
    board[row][col] = '0';
    for (int drow = -1; drow <= 1; ++drow)
    for (int dcol = -1; dcol <= 1; ++dcol)
        blinks += floodBlink(board, row + drow, col + dcol);

    return blinks;
}

int main() {
    std::vector<std::string> board;
    std::string next;

    while (std::getline(std::cin, next) && !next.empty()) {
        board.push_back(next);
    }

    int w = board[0].size();
    int h = board.size();

    int blinks = 0;
    for (int turn = 0; turn < 100; ++turn) {
        for (auto& row : board)
            for (auto& f : row)
                f += 1;
        
        for (int row : std::views::iota(0, h))
            for (int col : std::views::iota(0, w))
                blinks += board[row][col] > '9' ? floodBlink(board, row, col) : 0;
        
        // std::cout << "Turn " << turn + 1 << ":" << std::endl;
        // for (auto&s : board)
        //     std::cout << s << std::endl;
        // std::cout << std::endl;
    }
    std::cout << blinks;
}
