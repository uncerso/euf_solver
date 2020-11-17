#include "numerator.hpp"
#include "parser.hpp"
#include "dsu.hpp"
#include <iostream>
#include <string>

using namespace std;

namespace std {

template<> struct hash<pair<size_t, size_t>> {
    size_t operator()(pair<size_t, size_t> const & pr) const noexcept {
        std::hash<size_t> value_hasher;
        size_t seed = value_hasher(pr.first);
        seed ^= value_hasher(pr.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

}

struct Func {
    size_t id;
    FuncWithArgs const & func;
};

void merge(DSU & dsu, FuncToId const & func_to_id) {
    unordered_map<pair<size_t, size_t>, vector<Func> > func_classes;
    for (auto const & f : func_to_id)
        func_classes[{f.first.front(), f.first.size()}].push_back({f.second, f.first});
    bool merged = true;
    while (merged) {
        merged = false;
        for (auto const & f : func_classes) {
            auto const & funcs = f.second;
            for (size_t i = 0; i + 1 < funcs.size(); ++i) {
                for (size_t j = i + 1; j < funcs.size(); ++j) {
                    if (dsu.find(funcs[i].id) == dsu.find(funcs[j].id))
                        continue;
                    auto const & a = funcs[i].func;
                    auto const & b = funcs[j].func;
                    [&] {
                    for (size_t i = 1; i < a.size(); ++i)
                        if (dsu.find(a[i]) != dsu.find(b[i]))
                            return;
                    dsu.merge(funcs[i].id, funcs[j].id);
                    merged = true;
                    }();
                }
            }
        }
    }
}

int main() {
    try {
    string formula;
    getline(cin, formula);
    Numerator nm;
    auto terms = parse(formula, nm);
    DSU dsu(nm.total_elements());

    for (auto [lhs, rhs] : terms.equalities)
        dsu.merge(lhs, rhs);

    merge(dsu, nm.get_func_to_id_map());

    for (auto [lhs, rhs] : terms.inequalities) {
        if (dsu.find(lhs) == dsu.find(rhs)) {
            cout << "UNSAT\n";
            return 0;
        }
    }

    cout << "SAT\n";

    } catch (exception const & e) {
        cerr << e.what() << '\n';
        return 1;
    } catch (...) {
        cerr << "Unknown error :(\n";
        return 2;
    }
    return 0;
}
