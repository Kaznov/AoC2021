#include "utils.hh"

int main() {
    vec<int> input;
    utils::input_vector(input);

    int increasing_count = 0;
    for (size_t i = 0; i < input.size() - 1; ++i) {
        increasing_count += input[i] < input[i + 1];
    }

    cout << "Part 1: " << increasing_count << "\n";


    int increasing_triplets_count = 0;
    for (size_t i = 0; i < input.size() - 3; ++i) {
        increasing_triplets_count += input[i] < input [i + 3];
    }

    std::cout << "Part 2: " << increasing_triplets_count << "\n";
}
