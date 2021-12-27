#include "utils.hh"

int main() {
    int xmin, xmax, ymin, ymax;
    utils::input("target area: x={}..{}, y={}..{}", xmin, xmax, ymin, ymax);

    int maxvy = limits<int>::min();
    int distinct = 0;
    for (int vx0 = 1; vx0 <= xmax; ++vx0)
    for (int vy0 = ymin; vy0 <= -ymin; ++vy0) {
        int x = 0, y = 0, vx = vx0, vy = vy0;
        while (x <= xmax && y >= ymin) {
            if (x >= xmin && y <= ymax) {
                maxvy = std::max(maxvy, vy0);
                ++distinct;
                // std::cout << vx0 << "," << vy0 << "\n";
                break;
            }
            if (vx == 0 && x < xmin) break;
            x += vx;
            y += vy;
            vx = std::max(0, vx - 1);
            vy = vy - 1;
        }
    }

    std::cout << "Part 1: " << maxvy * (maxvy + 1) / 2 << "\n";
    std::cout << "Part 2: " << distinct << "\n";
}
