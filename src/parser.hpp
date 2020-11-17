#pragma once
#include "numerator.hpp"
#include <vector>
#include <utility>
#include <string_view>

struct Terms {
    using Term = std::pair<size_t, size_t>;
    std::vector<Term> equalities;
    std::vector<Term> non_equalities;
};



Terms parse(std::string_view formula, Numerator & numerator);