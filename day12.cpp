#include "utils.hh"

struct Cave {
    bool is_smol;
    string name;
    vec<int> links;
};

int main() {
    vec<Cave> graph;
    hashmap<string, int> ids;
    auto addCave = [&](string name) -> int {
        int id = graph.size();
        graph.emplace_back(islower(name[0]), name);
        ids[name] = id;
        return id;
    };

    auto getId = [&](string name) -> int {
        auto it = ids.find(name);
        if (it != ids.end()) return it->second;
        return addCave(name);
    };

    addCave("start");
    addCave("end");

    string in, out;
    while (std::getline(std::cin, in, '-') && std::getline(std::cin, out)) {
        int id1 = getId(in);
        int id2 = getId(out);
        graph[id1].links.push_back(id2);
        graph[id2].links.push_back(id1);
    }

    struct QEntry {
        int position;
        std::vector<int> visited;
    };

    std::queue<QEntry> q1, q2;
    QEntry start{ 0, std::vector<int>(graph.size()) };
    start.visited[0] = true;
    q1.push(start);

    int paths = 0;
    while (!q1.empty()) {
        auto entry = q1.front();
        q1.pop();
        if (entry.position == 1) {
            ++paths;
            continue;
        }

        for (int link : graph[entry.position].links) {
            bool is_smol = graph[link].is_smol;
            if (!is_smol || !entry.visited[link] || (link > 1)) {
                bool snd_visit = (is_smol && entry.visited[link]);
                auto visited_copy = entry.visited;

                visited_copy[link] = true;
                (snd_visit ? q2 : q1).emplace(link, visited_copy);
            }
        }
    }

    cout << "Part 1: " << paths << "\n";

    while (!q2.empty()) {
        auto entry = q2.front();
        q2.pop();
        if (entry.position == 1) {
            ++paths;
            continue;
        }

        for (int link : graph[entry.position].links) {
            bool is_smol = graph[link].is_smol;
            if (!is_smol || !entry.visited[link]) {
                auto visited_copy = entry.visited;
                visited_copy[link] = true;
                q2.emplace(link, visited_copy);
            }
        }
    }

    cout << "Part 2: " << paths << "\n";
}
