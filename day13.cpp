#include "utils.hh"

struct Fold {
    bool overx;
    int pos;
};

struct Dot {
    int x, y;
    auto operator<=>(const Dot& other) const = default;
};

int main() {
    set<Dot> dots;
    int x, y;
    while (utils::input_line("{},{}", x, y).success)
        dots.emplace(x, y);

    vec<Fold> folds;
    int pos; char how;
    while (utils::input_line("fold along {}={}", how, pos).success)
        folds.emplace_back(how == 'x', pos);

    auto apply_fold = [&dots](Fold fold) {
        set<Dot> dots2;
        for (auto dot : dots) {
            auto folded = [at = fold.pos] (int v) {
                return v >= at ? 2 * at - v : v;
            };
            if (fold.overx)
                dots2.emplace(folded(dot.x), dot.y);
            else
                dots2.emplace(dot.x, folded(dot.y));
        }
        swap(dots, dots2);
    };

    apply_fold(folds.front());
    cout << "Part 1: " << dots.size() << "\n";

    for (auto&& fold : folds | stdv::drop(1))
        apply_fold(fold);

    int maxx = stdr::max_element(dots, stdr::less{}, &Dot::x)->x;
    int maxy = stdr::max_element(dots, stdr::less{}, &Dot::y)->y;
    utils::Board<char> board(maxx + 1, maxy + 1, ' ');

    for (auto dot : dots)
        board.getv(dot.y, dot.x) = '#';

    cout << "Part 2: " << "\n";
    board.print();
}
