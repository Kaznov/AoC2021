#include "utils.hh"

constexpr int hallwayLength = 11;
constexpr int chamberCount = 4;
constexpr int maxHeight = 4;
constexpr int chamberPos[] = {2, 4, 6, 8};
constexpr int mCost[] = {1, 10, 100, 1000};

struct State {
    char top[hallwayLength] = {};
    char chambers[chamberCount][maxHeight]{}; // [x][y], y is top to bottom
    int height = 2;
    u64 cost = 0;

    friend std::ostream& operator<<(std::ostream& out, const State& state) {
        auto norm = [](char c) { return c == 0 ? '.' : c; };
        out << "#";
        for (auto c : state.top)
            out << norm(c);
        out << "#\n";

        for (int depth = 0; depth < state.height; ++depth) {
            out << "  #";
            for (const auto& chamber: state.chambers)
                out << norm(chamber[depth]) << "#";
            out << "\n";
        }

        out << "Cost: " << state.cost << "\n\n";
        return out;
    }
};

struct StateCompressed {
    u64 top;
    u64 chambers;
    StateCompressed(const State& s) {
        top = 0; chambers = 0;
        auto norm = [](char c) { return c == 0 ? 0 : c - 'A' + 1; };
        for (int i = 0; i < hallwayLength; ++i) {
            top <<= 3;
            top |= norm(s.top[i]);
        }

        for (int depth = 0; depth < s.height; ++depth)
        for (int c = 0; c < chamberCount; ++c) {
            chambers <<= 3;
            chambers |= norm(s.chambers[c][depth]);
        }
    }
    inline auto operator<=>(const StateCompressed& other) const = default;
};

bool is_finished(const State& s) {
    for (int chamber_id = 0; chamber_id < chamberCount; ++chamber_id) {
        char target = 'A' + chamber_id;
        auto& chamber = s.chambers[chamber_id];

        for (int depth = 0; depth < s.height; ++depth)
            if (chamber[depth] != target) return false;
    }
    return true;
}

std::vector<State> generateMoves(State s) {
    std::vector<State> result;
    auto chamber_good = [&](const auto& chamber, char target) {
        return stdr::all_of(chamber, chamber + s.height,
            [=](char locator) { return locator == 0 || locator == target; });
    };

    auto enter_chamber = [&](int type, int initial_cost, int initial_pos,
                             int target_pos, char& origin) {
        int c2 = (target_pos - 2) / 2;
        if (type != c2) return;
        auto& chamber2 = s.chambers[c2];
        char target2 = 'A' + c2;
        if (chamber2[0]) return; // is full
        if (!chamber_good(chamber2, target2)) return;

        // we can get in!
        int depth2 = 0;
        while (depth2 < s.height && chamber2[depth2] == 0) ++depth2;
        --depth2; // we want depth of the free position, not filled one

        std::swap(origin, chamber2[depth2]);
        int new_cost = s.cost;
        new_cost += initial_cost;
        new_cost += std::abs(target_pos - initial_pos) * mCost[type];
        new_cost += (depth2 + 1) * mCost[type];
        result.push_back(s);
        result.back().cost = new_cost;
        std::swap(origin, chamber2[depth2]); // restore state
    };

    auto enter_hallway = [&](int type, int initial_cost, int initial_pos,
                             int target_pos, char& origin) {
        std::swap(origin, s.top[target_pos]);
        int new_cost = s.cost;
        new_cost += initial_cost;
        new_cost += std::abs(target_pos - initial_pos) * mCost[type];
        result.push_back(s);
        result.back().cost = new_cost;
        std::swap(origin, s.top[target_pos]); // restore state
    };

    // try move out of chamber
    for (int c = 0; c < chamberCount; ++c) {
        auto& chamber = s.chambers[c];
        int depth = 0;
        while (depth < s.height && chamber[depth] == 0) ++depth;
        if (depth == s.height) continue;

        char target = 'A' + c;
        if (chamber_good(chamber, target))
            continue;

        int type = chamber[depth] - 'A';
        int initial_cost = (depth + 1) * mCost[type];
        int initial_pos = chamberPos[c];
        for (int h = initial_pos + 1; h < hallwayLength; ++h) {
            if (s.top[h]) break;
            if (h % 2 == 0 && h != 10) {
                enter_chamber(type, initial_cost, initial_pos, h, chamber[depth]);
            }
            else {
                enter_hallway(type, initial_cost, initial_pos, h, chamber[depth]);
            }
        }
        for (int h = initial_pos - 1; h >= 0; --h) {
            if (s.top[h]) break;
            if (h % 2 == 0 && h != 0) {
                enter_chamber(type, initial_cost, initial_pos, h, chamber[depth]);
            }
            else {
                enter_hallway(type, initial_cost, initial_pos, h, chamber[depth]);
            }
        }
    }

    // try move out of hallway
    for (int h = 0; h < hallwayLength; ++h) {
        if (!s.top[h]) continue;
        int type = s.top[h] - 'A';
        for (int h2 = h + 1; h2 <= 8; ++h2) {
            if (s.top[h2]) break;
            if (h2 % 2 == 0)
                enter_chamber(type, 0, h, h2, s.top[h]);
        }
        for (int h2 = h - 1; h2 >= 2; --h2) {
            if (s.top[h2]) break;
            if (h2 % 2 == 0)
                enter_chamber(type, 0, h, h2, s.top[h]);
        }
    }
    return result;
}

int main() {
    State start;
    auto& chs = start.chambers;
    utils::input_ignore_line();
    utils::input_ignore_line();
    utils::input_line("###{}#{}#{}#{}###", chs[0][0], chs[1][0], chs[2][0], chs[3][0]);
    utils::input_line("  #{}#{}#{}#{}#  ", chs[0][1], chs[1][1], chs[2][1], chs[3][1]);
    utils::input_ignore_line();

    auto solve = [](State start_state) {
        std::priority_queue<State, std::vector<State>,
                            decltype([](const State& s1, const State& s2){
                                return s1.cost > s2.cost;
                            })> pq;
        std::map<StateCompressed, u64> visited;
        pq.push(start_state);
        visited[start_state] = 0;

        while (true) {
            assert(!pq.empty());
            State state = pq.top();
            pq.pop();
            if (is_finished(state)) {
                return state.cost;
            }

            auto moves = generateMoves(state);
            for (auto move : moves) {
                StateCompressed move_id = move;
                if (!visited.contains(move_id) || visited[move_id] > move.cost) {
                    pq.push(move);
                    visited[move_id] = move.cost;
                }
            }
        }
    };

    std::cout << "Part 1: " << solve(start) << "\n";

    // add 2 extra rows in the middle
    start.height = 4;
    start.chambers[0][3] = start.chambers[0][1];
    start.chambers[1][3] = start.chambers[1][1];
    start.chambers[2][3] = start.chambers[2][1];
    start.chambers[3][3] = start.chambers[3][1];
    start.chambers[0][1] = 'D';
    start.chambers[1][1] = 'C';
    start.chambers[2][1] = 'B';
    start.chambers[3][1] = 'A';
    start.chambers[0][2] = 'D';
    start.chambers[1][2] = 'B';
    start.chambers[2][2] = 'A';
    start.chambers[3][2] = 'C';

    std::cout << "Part 2: " << solve(start) << "\n";
}
