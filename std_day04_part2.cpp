#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Bingo {
    bool marked[5][5]{};
    int values[5][5]{};
    bool finished = false;
    bool rowFull(size_t row) {
        for (int col = 0; col < 5; ++col)
            if (!marked[row][col]) return false;
        finished = true;
        return true;
    }
    bool colFull(size_t col) {
        for (int row = 0; row < 5; ++row)
            if (!marked[row][col]) return false;
        finished = true;
        return true;
    }
    int sumUnmarked() const {
        int sum = 0;
        for (int row = 0; row < 5; ++row)
        for (int col = 0; col < 5; ++col)
            sum += (!marked[row][col]) * values[row][col];
        return sum;
    }
};

Bingo readBoard() {
    Bingo bingo;
    for (int row = 0; row < 5; ++row)
    for (int col = 0; col < 5; ++col)
        std::cin >> bingo.values[row][col];
    std::cin.ignore(16, '\n');
    return bingo;
}

int main() {
    std::string random_nums_text;
    std::getline(std::cin, random_nums_text);

    std::istringstream random_nums_ss(random_nums_text);
    std::vector<int> random_nums;
    // repeat: read number, ignore character (','), as long as it works
    for (int next; random_nums_ss >> next; random_nums_ss.ignore()) {
        random_nums.push_back(next);
    }

    std::vector<Bingo> boards;
    while (std::cin.peek() != EOF && std::cin) {
        boards.push_back(readBoard());
    }

    std::unordered_map<int, std::vector<int>> hits;
    for (size_t b_id = 0; b_id < boards.size(); ++b_id)
    for (int row = 0; row < 5; ++row)
    for (int col = 0; col < 5; ++col)
            hits[boards[b_id].values[row][col]].push_back(b_id);

    int last_winner_score = 0;
    int boards_finished = 0;
    for (int num : random_nums) {
        for (int board_hit : hits[num]) {
            Bingo& board = boards[board_hit];
            if (board.finished) continue;
            // find the value, mark it
            for (int row = 0; row < 5; ++row)
            for (int col = 0; col < 5; ++col)
                if (board.values[row][col] == num) {
                    board.marked[row][col] = true;
                    if (board.rowFull(row) || board.colFull(col)) {
                        last_winner_score = board.sumUnmarked() * num;
                        ++boards_finished;
                    }
                }
        }
        if (boards_finished == static_cast<int>(boards.size())) break;
    }

    std::cout << "Part 2: " << last_winner_score << "\n";
}
