#define __cplusplus 201703L
#pragma once
#include <unordered_map>
#include <string_view>
#include <vector>

using FuncWithArgs = std::vector<size_t>;

namespace std {

template<> struct hash<FuncWithArgs> {
    size_t operator()(FuncWithArgs const & vec) const noexcept {
        std::hash<size_t> value_hasher;
        size_t seed = vec.size();
        for(auto const & i : vec)
            seed ^= value_hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

}

using FuncToId = std::unordered_map<FuncWithArgs, size_t>;

class Numerator {
    using str_t = std::string_view;
    size_t next_free_id = 0;
    std::unordered_map<str_t, size_t> name_to_id;
    FuncToId func_with_args_to_id;

public:
    size_t total_elements() const noexcept { return next_free_id; }
    FuncToId const & get_func_to_id_map() const noexcept { return func_with_args_to_id; }

    size_t get_id(str_t s) {
        auto [it, ok] = name_to_id.emplace(s, next_free_id);
        if (!ok)
            return it->second;
        return next_free_id++;
    }

    size_t get_id(FuncWithArgs func_with_args) {
        auto [it, ok] = func_with_args_to_id.emplace(std::move(func_with_args), next_free_id);
        if (!ok)
            return it->second;
        return next_free_id++;
    }
};
