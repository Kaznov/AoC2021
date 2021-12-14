#include <algorithm>
#include <iostream>
#include <iterator>
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

std::vector<std::string> splitText(std::string text, char sep = ' ') {
    size_t pos = text.find(sep);
    size_t initial_pos = 0;
    std::vector<std::string> result;

    while (pos != std::string::npos) {
        result.emplace_back(text.substr(initial_pos, pos - initial_pos));
        initial_pos = pos + 1;
        pos = text.find(sep, initial_pos);
    }

    // Add the last one
    result.push_back(text.substr(initial_pos));

    return result;
}

int main() {
    std::string random_nums_text;
    std::getline(std::cin, random_nums_text);
    auto random_nums_split = splitText(random_nums_text, ',');
    std::vector<int> random_nums;
    std::transform(std::begin(random_nums_split), std::end(random_nums_split),
                   std::back_inserter(random_nums),
                   [](auto&& s) -> int { return std::stoi(s); });

    std::vector<Bingo> boards;
    while (std::cin.peek() != EOF && std::cin) {
        boards.push_back(readBoard());
    }

    std::unordered_map<int, std::vector<int>> hits;
    for (size_t b_id = 0; b_id < boards.size(); ++b_id)
    for (int row = 0; row < 5; ++row)
    for (int col = 0; col < 5; ++col)
            hits[boards[b_id].values[row][col]].push_back(b_id);

    int winner_score = 0;
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
                        winner_score = board.sumUnmarked() * num;
                        ++boards_finished;
                    }
                }
        }
        if (boards_finished == static_cast<int>(boards.size())) break;
    }

    std::cout << winner_score;
}
