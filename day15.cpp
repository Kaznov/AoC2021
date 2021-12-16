#include "utils.hh"

using utils::Board;
using utils::Point;

int main() {
    struct BoardEntry { int cost; bool visited; };

    auto b = utils::read_board<char>()
        .projected([](char c) -> BoardEntry {
            return { c - '0', false }; });
    int w = b.width;
    int h = b.height;

    auto best_distance = [](Board<BoardEntry>& b) {
        struct QueueEntry {
            int distance;
            Point<> position;
            auto operator<=>(const QueueEntry& other) const = default;
        };

        priority_queue<QueueEntry, std::vector<QueueEntry>, std::greater<>> q;
        q.push({ 0, Point{ 0, 0 } });
        Point<> target = { b.width - 1, b.height - 1 };

        while (!q.empty() && q.top().position != target) {
            auto p = q.top(); q.pop();
            auto field = b.get(p.position.y, p.position.x);

            if (field.v.get().visited) continue; // already visited
            field.v.get().visited = true;

            auto adj = b.adjacent(field);
            for (auto&& adjfield : adj)
                q.push({ adjfield.v.get().cost + p.distance, adjfield.to_point() });
        }
        return q.top().distance;
    };

    cout << "Part 1: " << best_distance(b) << "\n";

    Board<BoardEntry> b2(5 * w, 5 * h);
    for (int srow = 0; srow < 5; ++srow)
    for (int scol = 0; scol < 5; ++scol)
    for (int row = 0; row < h; ++row)
    for (int col = 0; col < w; ++col) {
        int inc = srow + scol;
        int new_cost = b.getv(row, col).cost + inc;
        new_cost = (new_cost - 1) % 9 + 1;
        b2.getv(srow * h + row, scol * w + col).cost = new_cost;
    }

    cout << "Part 2: " << best_distance(b2) << "\n";
}
