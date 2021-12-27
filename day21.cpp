#include <cstdint>
#include <iostream>
#include <utility>

using u64 = uint64_t;
struct State {
    int pos[2];
    int score[2];
    int turn;

    State afterRoll(int roll) const {
        State next = *this;
        next.pos[turn] = (pos[turn] + roll) % 10;
        next.score[turn] += next.pos[turn] + 1;
        next.turn = !turn;
        return next;
    }
};

struct DeterministicDie {
    int die = -1;
    int throws = 0;
    int roll3() {
        int inc = 0;
        inc += die = ((die + 1) % 10);
        inc += die = ((die + 1) % 10);
        inc += die = ((die + 1) % 10);
        throws += 3;
        inc += 3;
        return inc;
    }
};

std::pair<u64, u64> cache[2][10][10][21][21] {};
std::pair<u64, u64>& getCache(const State& s) {
    return cache[s.turn][s.pos[0]][s.pos[1]][s.score[0]][s.score[1]];
}
constexpr std::pair<u64, u64> empty = {};

std::pair<u64, u64> getDiracResults(const State& s) {
    if (s.score[0] >= 21) return {1, 0};
    if (s.score[1] >= 21) return {0, 1};
    {
        auto cached = getCache(s);
        if (cached != empty)
            return cached;
    }

    int probs[7] = {1, 3, 6, 7, 6, 3, 1};
    std::pair<u64, u64> res{};
    for (int i = 0; i < 7; ++i) {
        auto subresult =
            getDiracResults(s.afterRoll(i + 3));
        res.first += probs[i] * subresult.first;
        res.second += probs[i] * subresult.second;
    }

    getCache(s) = res;
    return res;
}

int main() {
    int pos1, pos2;
    std::cin >> pos1 >> pos2;
    State start = {{pos1 - 1, pos2 - 1}};
    DeterministicDie die;

    State current = start;
    while (current.score[0] < 1000 && current.score[1] < 1000) {
        current = current.afterRoll(die.roll3());
    }
    std::cout << "Part 1: " << std::min(current.score[0], current.score[1]) * die.throws << "\n";

    auto universes = getDiracResults(start);
    std::cout << "Part 2: " << std::max(universes.first, universes.second);
}
