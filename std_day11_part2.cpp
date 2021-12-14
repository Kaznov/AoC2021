#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

int floodBlink(std::vector<std::string>& board, int row, int col) {
    if (row < 0 || col < 0 || row >= board.size() || col >= board[0].size())
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

    int turn = 0;
    while (true) {
        ++turn;
        for (auto& row : board)
            for (auto& f : row)
                f += 1;
        
        for (int row : std::views::iota(0, h))
            for (int col : std::views::iota(0, w))
                if (board[row][col] > '9') floodBlink(board, row, col);

        if (std::ranges::all_of(board, [](auto& row) -> bool {
            return std::ranges::all_of(row, [](char c) { return c == '0'; });
        })) break;
    }
    
    std::cout << turn;
}
