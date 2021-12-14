#include "utils.hh"

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
    vec<int> random_nums;
    utils::input_line_vector(random_nums, ",");

    vec<Bingo> boards;
    while (std::cin.peek() != EOF && std::cin) {
        boards.push_back(readBoard());
    }

    int winner_score = 0;
    int loser_score = 0;
    size_t boards_finished = 0;
    for (int num : random_nums) {
        for (Bingo& board : boards) {
            if (board.finished) continue;
            // find the value, mark it
            for (int row = 0; row < 5; ++row)
            for (int col = 0; col < 5; ++col)
                if (board.values[row][col] == num) {
                    board.marked[row][col] = true;
                    if (board.rowFull(row) || board.colFull(col)) {
                        if (winner_score == 0)
                            winner_score = board.sumUnmarked() * num;
                        loser_score = board.sumUnmarked() * num;
                        ++boards_finished;
                    }
                }
        }
        if (boards_finished == boards.size()) break;
    }

    std::cout << "Part 1: " << winner_score << "\n"
              << "Part 2: " << loser_score << "\n";
}
