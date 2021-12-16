#include "utils.hh"

struct BitStream {
    u64 consumeBits(int count) {
        assert(count > 0 && count + stream_pos <= data.size());
        u64 result = 0;
        for (int i = 0; i < count; ++i) {
            result <<= 1;
            result |= data[stream_pos++];
        }
        return result;
    }

    size_t stream_pos = 0;
    vec<bool> data;
};

struct Packet {
    u8 version = 0xFF;
    u8 typeID  = 0xFF;
    u64 literal = 0;
    vec<Packet> subpackets;

    u64 versionSum() const {
        return version + utils::sum(subpackets, u64{ 0 }, &Packet::versionSum);
    }

    u64 evaluate() const {
        // !!! stdr::min and stdr::max evaluate projections multiple times !!!
        // so while nicely looking, it is very inefficient
        switch (typeID) {
        case 0:
            return utils::sum(subpackets, u64{ 0 }, &Packet::evaluate);
        case 1:
            return utils::product(subpackets, u64{ 1 }, &Packet::evaluate);
        case 2:
            return stdr::min(subpackets, stdr::less{}, &Packet::evaluate).evaluate();
        case 3:
            return stdr::max(subpackets, stdr::less{}, &Packet::evaluate).evaluate();
        case 4:
            return literal;
        case 5:
            return subpackets[0].evaluate() > subpackets[1].evaluate();
        case 6:
            return subpackets[0].evaluate() < subpackets[1].evaluate();
        case 7:
            return subpackets[0].evaluate() == subpackets[1].evaluate();
        default:
            throw;
        }
    }
};

Packet readPacket(BitStream& bs) {
    Packet p;
    p.version = bs.consumeBits(3);
    p.typeID = bs.consumeBits(3);
    if (p.typeID == 4) {
        uint next_digit = 0;
        p.literal = 0;
        do {
            next_digit = bs.consumeBits(5);
            p.literal <<= 4;
            p.literal |= next_digit & 0xF;
        } while(next_digit > 15);
    } else {
        uint length_id = bs.consumeBits(1);
        uint size_limit = length_id == 0
                        ? bs.consumeBits(15)
                        : limits<uint>::max();
        uint count_limit = length_id == 1
                         ? bs.consumeBits(11)
                         : limits<uint>::max();
        size_t pos_limit = bs.stream_pos + size_limit;

        for (uint i = 0; i < count_limit && bs.stream_pos < pos_limit; ++i) {
            p.subpackets.push_back(readPacket(bs));
        }
    }

    return p;
}

int main() {
    string hexstream;
    cin >> hexstream;
    BitStream bs;
    for (char c  : hexstream) {
        uint digit = c >= '0' && c <= '9' ? c - '0' : c - 'A' + 10;
        bs.data.push_back(digit & 8);
        bs.data.push_back(digit & 4);
        bs.data.push_back(digit & 2);
        bs.data.push_back(digit & 1);
    }

    Packet outermost = readPacket(bs);

    cout << "Part 1: " << outermost.versionSum() << "\n";
    cout << "Part 2: " << outermost.evaluate() << "\n";
}
