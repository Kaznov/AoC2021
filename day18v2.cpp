#include "utils.hh"

template<typename T>
using UP = std::unique_ptr<T>;

struct SnailNum {
    UP<SnailNum> left, right;
    int height = 1;
    int value = 0;
    int max = 0;

    SnailNum() = default;

    SnailNum(UP<SnailNum> l, UP<SnailNum> r)
        : left(std::move(l)), right(std::move(r))
        { fixNode(); }

    SnailNum(int v) : value(v), max(v) { fixNode(); }

    friend std::istream& operator>>(std::istream& in, SnailNum& out) {
        auto consumeChar = [&in](char c) {
            auto peeked = in.peek();
            if(in.peek() != c)
                throw;
            in.ignore();
        };
        if(in.peek() == '[') {
            consumeChar('[');
            auto left = std::make_unique<SnailNum>();
            auto right = std::make_unique<SnailNum>();
            in >> *left;
            consumeChar(',');
            in >> *right;
            out = SnailNum(std::move(left), std::move(right));
            consumeChar(']');
        }
        else {
            in >> out.value;
        }
        out.fixNode();
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const SnailNum& num) {
        if (!num.left && !num.right)
            return out << num.value;
        else
            return out << "(" << *num.left << "," << *num.right << ")";
                    // << "[" << num.height << "," << num.max << "]";
    }

    SnailNum clone() const {
        if (isLeaf()) return SnailNum{ value };
        else return SnailNum{ std::make_unique<SnailNum>(left->clone()),
                              std::make_unique<SnailNum>(right->clone()) };
    }

    bool isLeaf() const {
        assert(!!left == !!right);
        return left == nullptr;
    }

    void fixNode() {
        height = isLeaf() ? 1 : std::max(left->height, right->height) + 1;
        max = isLeaf() ? value : std::max(left->max, right->max);
    }

    bool explode() {
        if (height >= 6) {
            explodeRec(4); return true;
        }
        return false;
    }

    void addToLeftmost(int v) {
        if (height == 1) value += v;
        else left->addToLeftmost(v);
        fixNode();
    }

    void addToRightmost(int v) {
        if (height == 1) value += v;
        else right->addToRightmost(v);
        fixNode();
    }

    std::pair<int, int> explodeRec(int at_depth) {
        std::pair<int, int> exploded;
        if (at_depth == 0) {
            exploded = {left->value, right->value};
            *this = {};
        }
        else {
            if (left->height >= right->height) {
                exploded = left->explodeRec(at_depth - 1);
                if (exploded.second) {
                    right->addToLeftmost(exploded.second);
                    exploded.second = 0;
                }
            }
            else {
                exploded = right->explodeRec(at_depth - 1);
                if (exploded.first) {
                    left->addToRightmost(exploded.first);
                    exploded.first = 0;
                }
            }
        }
        fixNode();
        return exploded;
    }

    bool split() {
        bool result = false;
        if (isLeaf() && value >= 10) {
            int v = value;
            value = 0;
            left = std::make_unique<SnailNum>(v / 2);
            right = std::make_unique<SnailNum>((v + 1) / 2);
            result = true;
        }
        else if (left->max >= 10) {
            left->split();
            result = true;
        } else if (right->max >= 10) {
            right->split();
            result = true;
        }
        fixNode();
        return result;
    }

    int magnitude() const {
        if (!left && !right) return value;
        return 3 * left->magnitude() + 2 * right->magnitude();
    }
};

SnailNum reduce(SnailNum&& left, SnailNum&& right) {
    SnailNum result(std::make_unique<SnailNum>(std::move(left)),
                    std::make_unique<SnailNum>(std::move(right)));
    while (result.explode() || result.split()) { }

    return result;
}

int main() {
    std::vector<SnailNum> snail_nums;
    std::string line;
    while (getline(cin, line)) {
        SnailNum num;
        std::istringstream{line} >> num;
        snail_nums.push_back(std::move(num));
    }

    SnailNum accumulator = snail_nums[0].clone();
    for (auto&& num : snail_nums | stdv::drop(1))
        accumulator = reduce(std::move(accumulator), num.clone());

    std::cout << "Part 1: " << accumulator.magnitude() << "\n";

    int max_magnitude = 0;
    for (size_t i = 0; i < snail_nums.size(); ++i)
    for (size_t j = 0; j < snail_nums.size(); ++j) {
        if (i == j) continue;
        max_magnitude =
            std::max(max_magnitude,
                     (reduce(snail_nums[i].clone(),
                             snail_nums[j].clone())).magnitude());
    }

    cout << "Part 2: " << max_magnitude << "\n";
}
