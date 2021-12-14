#include "utils.hh"

int main() {
    vec<int> input;
    utils::input_vector(input);
    vec<int> increases;
    std::adjacent_difference(std::begin(input), std::end(input),
                             std::back_inserter(increases),
                             stdr::greater{});
    cout << "Part 1: " << utils::sum(increases | stdv::drop(1))
         << "\n";

    int increasing_sum_count = 0;
    int last_moving_sum = 1000000;
    for (size_t i = 0; i < input.size() - 2; ++i) {
        int sum = input[i] + input[i + 1] + input[i + 2];
        if (sum > last_moving_sum) ++increasing_sum_count;
        last_moving_sum = sum;
    }
    cout << "Part 2: " << increasing_sum_count << "\n";
}
