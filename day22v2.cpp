#include "utils.hh"

struct Command {
    bool type;
    int xmin, xmax, ymin, ymax, zmin, zmax;
};

int main() {
    std::vector<Command> commands;
    {
        Command next;
        string next_type;
        while (utils::input_line("{} x={}..{},y={}..{},z={}..{}",
                next_type, next.xmin, next.xmax,
                next.ymin, next.ymax, next.zmin, next.zmax)) {
            // if (next.xmin < -50 || next.xmax > 50 ) continue;
            next.xmax++; next.ymax++; next.zmax++;
            next.type = next_type == "on";
            commands.push_back(next);
        }
    }

    std::set<int> xlines_set, ylines_set, zlines_set;
    for (const auto& c : commands) {
        xlines_set.insert(c.xmin);
        xlines_set.insert(c.xmax);
        ylines_set.insert(c.ymin);
        ylines_set.insert(c.ymax);
        zlines_set.insert(c.zmin);
        zlines_set.insert(c.zmax);
    }
    std::vector<int> xlines(xlines_set.begin(), xlines_set.end());
    std::vector<int> ylines(ylines_set.begin(), ylines_set.end());
    std::vector<int> zlines(zlines_set.begin(), zlines_set.end());

    u64 total_size = 0;

    for (int xid = 0; xid < xlines.size() - 1; xid++)
    for (int yid = 0; yid < ylines.size() - 1; yid++)
    for (int zid = 0; zid < zlines.size() - 1; zid++) {
        int x0 = xlines[xid];
        int x1 = xlines[xid + 1];
        int y0 = ylines[yid];
        int y1 = ylines[yid + 1];
        int z0 = zlines[zid];
        int z1 = zlines[zid + 1];
        bool lit = false;
        for (const auto& c : commands | stdv::reverse) {
            if (x0 >= c.xmin && x0 < c.xmax &&
                y0 >= c.ymin && y0 < c.ymax &&
                z0 >= c.zmin && z0 < c.zmax) {
                lit = c.type;
                break;
            }
        }
        if (lit)
            total_size += (u64)(x1 - x0) * (u64)(y1 - y0) * (u64)(z1 - z0);
    }

    std::cout << total_size << "\n";
}
