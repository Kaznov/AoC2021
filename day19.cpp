#include "utils.hh"

struct Point3D {
    int x, y, z;
    auto operator<=>(const Point3D&) const = default;
    friend std::ostream& operator<<(std::ostream& out, Point3D p) {
        return out << "("
                   << std::setw(5) << p.x << ","
                   << std::setw(5) << p.y << ","
                   << std::setw(5) << p.z << ") ";
    }
};

template<>
struct std::hash<Point3D> {
    size_t operator()(const Point3D& p) const
    {
        return ((17 * 31u + p.x) * 31u + p.y) * 31u + p.z;
    }
};

struct Scanner {
    int id = -1;
    bool adjusted = false;
    Point3D position{};
    std::vector<Point3D> beacons;
};

struct Counter
{
  struct value_type { template<typename T> value_type(const T&) { } };
  void push_back(const value_type&) { ++count; }
  size_t count = 0;
};

Scanner readScanner() {
    Scanner s;
    utils::input_line("--- scanner {} ---", s.id);
    std::string line;
    while (getline(cin, line) && !line.empty()) {
        int x, y, z;
        utils::scan(line, "{},{},{}", x, y, z);
        s.beacons.emplace_back(x, y, z);
    }
    return s;
}

void applyPermutation(vec<Point3D>& beacons, int perm[3]) {
    for (auto& p : beacons) {
        int ps[3] = {p.x, p.y, p.z};
        int ps2[3] = {ps[perm[0]], ps[perm[1]], ps[perm[2]]};
        p.x = ps2[0];
        p.y = ps2[1];
        p.z = ps2[2];
    }
}

void applyNegation(vec<Point3D>& beacons, int neg) {
    int muls[3] = { neg & 1 ? -1 : 1, neg & 2 ? -1 : 1, neg & 4 ? -1 : 1 };
    for (auto& p : beacons) {
        p.x *= muls[0];
        p.y *= muls[1];
        p.z *= muls[2];
    }
}

void applyOffset(vec<Point3D>& beacons, Point3D offset) {
    for (auto& b : beacons) {
        b.x -= offset.x;
        b.y -= offset.y;
        b.z -= offset.z;
    }
}

void adjustScanner(Scanner& scanner, const Scanner& known, int perm[3], int neg, Point3D offset) {
    applyPermutation(scanner.beacons, perm);
    applyNegation(scanner.beacons, neg);
    applyOffset(scanner.beacons, offset);

    scanner.position = offset;
    scanner.adjusted = true;
    cout << "Found scanner " << scanner.id << " intersecting with " << known.id << "\n";
    cout << scanner.position << "\n";
}

bool tryMatchingScanners(const Scanner& sknown, Scanner& sUnknown) {
    static vec<Point3D> known_beacons;
    known_beacons = sknown.beacons;
    hashset<Point3D> known_set{ known_beacons.begin(), known_beacons.end() };

    stdr::sort(known_beacons);

    for (int reversal = 0; reversal < 8; ++reversal) {
        int permutation[3] {0, 1, 2};
        do {
            static vec<Point3D> unknown_beacons;
            unknown_beacons = sUnknown.beacons;
            applyPermutation(unknown_beacons, permutation);
            applyNegation(unknown_beacons, reversal);

            for (const auto& p1 : known_beacons)
            for (const auto& p2 : unknown_beacons) {
                // D = U - K
                // U = K + D
                Point3D offset {p2.x-p1.x, p2.y-p1.y, p2.z-p1.z};
                static vec<Point3D> unknown_beacons_copy;
                unknown_beacons_copy = unknown_beacons;
                applyOffset(unknown_beacons_copy, offset);
                if (stdr::count_if(unknown_beacons_copy,
                                   [&](const Point3D& p) {
                                        return known_set.contains(p); })
                        >= 12) {
                    adjustScanner(sUnknown, sknown, permutation, reversal, offset);
                    return true;
                }
            }
        } while(stdr::next_permutation(permutation).found);
    }
    return false;
}

int main() {
    vec<Scanner> scanners;
    while (cin) {
        scanners.push_back(readScanner());
    }

    vec<std::reference_wrapper<Scanner>> adjusted = {scanners[0]};
    scanners[0].adjusted = true;
    for (int adj_pos = 0; adj_pos < adjusted.size(); ++adj_pos) {
        Scanner& known_scanner = adjusted[adj_pos];
        for (Scanner& unknown_scanner : scanners) {
            if (unknown_scanner.adjusted) continue;
            if (tryMatchingScanners(known_scanner, unknown_scanner)) {
                adjusted.push_back(std::ref(unknown_scanner));
            }
        }
    }

    std::set<Point3D> all_beacons;
    for (const auto& scanner : scanners) {
        all_beacons.insert(scanner.beacons.begin(),
                           scanner.beacons.end());
    }

    std::cout << "Part 1: " << all_beacons.size() << "\n";

    int max_distance = 0;
    for (const auto& s1 : scanners)
    for (const auto& s2 : scanners) {
        if (s1.id == s2.id) continue;
        max_distance = std::max(
            max_distance,
            std::abs(s1.position.x - s2.position.x)
                + std::abs(s1.position.y - s2.position.y)
                + std::abs(s1.position.z - s2.position.z));
    }
    std::cout << "Part 2: " << max_distance << "\n";
}
