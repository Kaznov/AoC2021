#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> board;
    std::string next;

    while (std::getline(std::cin, next) && !next.empty()){
        next.insert(next.begin(), '9');
        next.push_back('9');
        board.push_back(next);
    }

    int w = board[0].size();
    int h = board.size() + 2;
    board.insert(board.begin(), std::string(w, '9'));
    board.push_back(std::string(w, '9'));

    int sum = 0;
    auto level = [&](int i, int j) { return board[i][j] - '0' + 1; };

    for (int i = 1; i < h - 1; ++i) {
        for (int j = 1; j < w - 1; ++j) {
            int tl = level(i, j);
            int inc = tl * (tl < level(i - 1, j) &&
                            tl < level(i, j - 1) &&
                            tl < level(i + 1, j) &&
                            tl < level(i, j + 1));
            sum += inc;
        }
    }
    std::cout << sum;
}
