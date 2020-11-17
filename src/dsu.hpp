#pragma once
#include <vector>

class DSU {
    constexpr static size_t npos = -1ull;
    std::vector<size_t> p;
    std::vector<size_t> size;
public:
    DSU(size_t amount_of_groups)
        : p(amount_of_groups, npos)
        , size(amount_of_groups, 1)
    {}

    size_t find(size_t e) {
        if (p[e] == npos)
            return e;
        return p[e] = find(p[e]);
    }

    void merge(size_t a, size_t b) {
        a = find(a);
        b = find(b);
        if (a == b)
            return;
        if (size[b] > size[a])
            std::swap(a,b);
        p[b] = a;
        size[a] += size[b];
    }
};
