#include "utils.hh"

struct Token {
    enum Type {
        open, close, comma, value
    } type;
    int v = 0;
};

using SnailNum = std::list<Token>;

void print(const SnailNum& l);

SnailNum tokenize(std::istringstream line) {
    SnailNum tokens;
    while (line && line.peek() != EOF) {
        switch (line.get()) {
            case '[':
                tokens.push_back({Token::open});
                break;
            case ']':
                tokens.push_back({Token::close});
                break;
            case ',':
                tokens.push_back({Token::comma});
                break;
            default: {
                Token t{Token::value};
                line.unget();
                line >> t.v;
                tokens.push_back(t);
            }
        }
    }
    return tokens;
}

bool explode(SnailNum& l) {
    auto it = stdr::find_if(
        l,
        [depth = 0](const Token::Type& type) mutable -> bool {
            if (type == Token::close) --depth;
            if (type == Token::open) ++depth;
            return depth > 4;
        },
        &Token::type);

    if (it == l.end())
        return false;

    auto consume = [&l, &it](Token::Type type) {
        assert(it->type == type);
        l.erase(it++);
    };

    auto prev = std::prev(it);
    consume(Token::open);
    int num1 = it->v;
    consume(Token::value);
    consume(Token::comma);
    int num2 = it->v;
    consume(Token::value);
    consume(Token::close);
    l.insert(it, {Token::value});

    auto prev_num_it = std::find_if(
        std::make_reverse_iterator(prev),
        l.rend(),
        [](Token t) { return t.type == Token::value;});
    if (prev_num_it != l.rend()) prev_num_it->v += num1;

    auto next_num_it = std::find_if(
        it, l.end(), [](Token t) { return t.type == Token::value;});
    if (next_num_it != l.end()) next_num_it->v += num2;

    return true;
}

bool split(SnailNum& l) {
    auto first_bigger =
        stdr::find_if(l, utils::greatereq_than<int>(10), &Token::v);

    if (first_bigger == l.end()) return false;
    auto next = std::next(first_bigger);
    int num = first_bigger->v;
    l.erase(first_bigger);
    l.insert(next, {Token::open});
    l.insert(next, {Token::value, num / 2});
    l.insert(next, {Token::comma});
    l.insert(next, {Token::value, (num + 1) / 2});
    l.insert(next, {Token::close});

    return true;
}

SnailNum reduce(SnailNum l1, SnailNum l2) {
    SnailNum lr;
    lr.push_back({Token::open});
    lr.splice(lr.end(), l1);
    lr.push_back({Token::comma});
    lr.splice(lr.end(), l2);
    lr.push_back({Token::close});
    while (explode(lr) || split(lr)) {
        // print(lr);
    }
    return lr;
}

int magnitude(const SnailNum& l) {
    vec<int> s;
    for (const auto& t : l) {
        switch (t.type) {
            case Token::open:
            case Token::comma:
                break;
            case Token::close: {
                int n2 = s.back(); s.pop_back();
                int n1 = s.back(); s.pop_back();
                s.push_back(3 * n1 + 2 * n2);
                break;
            }
            default:
                s.push_back(t.v);
        }
    }
    assert(s.size() == 1);
    return s.back();
}

void print(const SnailNum& l) {
    for (const auto& t : l) {
        switch (t.type) {
            case Token::open:
                cout << '[';
                break;
            case Token::close:
                cout << ']';
                break;
            case Token::comma:
                cout << ',';
                break;
            default: {
                cout << t.v;
            }
        }
    }
    cout << endl;
}

int main() {
    std::vector<SnailNum> snail_nums;
    std::string line;
    while (getline(cin, line)) {
        snail_nums.push_back(tokenize(std::istringstream{line}));
    }

    SnailNum accumulator = snail_nums[0];
    for (const auto& num : snail_nums | stdv::drop(1))
        accumulator = reduce(std::move(accumulator), num);

    cout << "Part 1: " << magnitude(accumulator) << "\n";

    int max_magnitude = 0;
    for (size_t i = 0; i < snail_nums.size(); ++i)
    for (size_t j = 0; j < snail_nums.size(); ++j) {
        if (i == j) continue;
        max_magnitude =
            std::max(max_magnitude,
                     magnitude(reduce(snail_nums[i], snail_nums[j])));
    }

    cout << "Part 2: " << max_magnitude << "\n";
}
