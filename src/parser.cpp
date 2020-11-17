#include "parser.hpp"
#include <string>
#include <stdexcept>

using namespace std;

namespace {

bool is_symbol(char c) {
    return ('a' <= c && c <= 'z') || 
           ('A' <= c && c <= 'Z') || 
           isdigit(c) || 
           c == '_'; 
}

struct SafeIterator {
    string_view s;
    size_t pos = 0;

    char operator * () const noexcept {
        return (is_end() ? 0 : s[pos]);
    }

    void operator ++ () noexcept {
        ++pos;
    }

    bool is_end() const noexcept {
        return s.size() <= pos;
    }
};

class TermExtractor : public SafeIterator{
    Numerator & numerator;

public: 
    TermExtractor(string_view term, Numerator & numerator)
        : SafeIterator{term, 0}
        , numerator(numerator)
    {}

    void skip_spaces() {
        while (isspace(**this))
            ++*this;
    }

    string_view get_name() noexcept {
        auto & it = *this;
        auto start = it.pos;
        while (is_symbol(*it))
            ++it;
        return it.s.substr(start, it.pos - start);
    }

    size_t extract() {
        auto & it = *this;
        auto name = get_name();
        if (name.empty())
            throw std::invalid_argument("Expected term at pos "s + to_string(it.pos) + " in '" + string(it.s) + '\'');
        auto ans = numerator.get_id(name);
        skip_spaces();
        if (*it != '(')
            return ans;
        
        FuncWithArgs func(1, ans);

        while (true) {
            ++it;
            skip_spaces();
            func.push_back(extract());
            if (*it == ')')
                break;
            if (*it != ',')
                throw std::invalid_argument("Expected ')' or ',' at pos "s + to_string(it.pos) + " in '" + string(it.s) + '\'');
        }
        ++it;
        return numerator.get_id(move(func));
    }
};

bool is_equality(TermExtractor & it) {
    auto pos = it.pos;
    auto c = *it;
    ++it;
    if (c == '=')
        return true;

    if (c != '!' || *it != '=')
        throw std::invalid_argument("Expected '!=' or '=' at pos "s + to_string(pos) + " in '" + string(it.s) + '\'');
    ++it;
    return false;
}

} // namespace

Terms parse(string_view formula, Numerator & numerator) {
    Terms terms;
    TermExtractor extractor(formula, numerator);
    extractor.skip_spaces();
    if (extractor.is_end())
        return terms;

    while (true) {
        auto lhs = extractor.extract();
        extractor.skip_spaces();
        bool is_eq = is_equality(extractor);
        extractor.skip_spaces();
        auto rhs = extractor.extract();

        if (is_eq)
            terms.equalities.emplace_back(lhs, rhs);
        else
            terms.non_equalities.emplace_back(lhs, rhs);

        extractor.skip_spaces();
        if (extractor.is_end())
            break;
        if (*extractor != '&')
            throw std::invalid_argument("Expected '&' or 'end of file' at pos "s + to_string(extractor.pos) + " in '" + string(extractor.s) + '\'');
        ++extractor;
        extractor.skip_spaces();
    }
    return terms;
}
