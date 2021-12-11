#include <algorithm>
#include <functional>
#include <iomanip>
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

    std::vector<int> fu (w * h);
    std::vector<int> fus (w * h, 1);

    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            fu[i*w + j] = i * w + j;

    auto fin = [&] (int p1) -> int {
        int rep = p1;
        while (rep != fu[rep]) rep = fu[rep];
        while (p1 != rep) {
            int pnext = fu[p1];
            fu[p1] = rep;
            p1 = pnext;
        }
        return rep;
    };
    auto uni = [&](int p1, int p2) {
        int rep1 = fin(p1), rep2 = fin(p2);
        if (rep1 == rep2) return;
        fu[rep2] = rep1;
        fus[rep1] += fus[rep2];
    };
    auto level = [&](int i, int j) { return board[i][j] - '0' + 1; };

    for (int i = 1; i < h; ++i) {
        for (int j = 1; j < w; ++j) {
            int l = level(i, j);
            if (l >= 10) continue;
            if (l > level(i - 1, j)) uni(i * w + j, (i - 1) * w + j);
            if (l > level(i, j - 1)) uni(i * w + j, i * w + j - 1);
            if (l > level(i + 1, j)) uni(i * w + j, (i + 1) * w + j);
            if (l > level(i, j + 1)) uni(i * w + j, i * w + j + 1);
        }
    }

    std::vector<int> regions;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            // cout << setw(4) << fus[fin(i * w + j)] << " ";
            if (fin(i*w + j) == i * w + j)
                regions.push_back(fus[i * w + j]);
        }
        // std::cout << std::endl;
    }
    std::ranges::partial_sort(regions, regions.begin() + 3, std::greater<>{});

    std::cout << regions[0] * regions[1] * regions[2];
}
