/*
 * SPDX-FileCopyrightText: 2017~2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#include "wenzhouneseencoder.h"
#include "wenzhounesedata.h"
#include <boost/algorithm/string.hpp>
#include <boost/bimap.hpp>
#include <queue>

namespace libime {
namespace wenzhounese {

static const std::string emptyString;

template <typename L, typename R>
boost::bimap<L, R>
makeBimap(std::initializer_list<typename boost::bimap<L, R>::value_type> list) {
    return boost::bimap<L, R>(list.begin(), list.end());
}

static const auto initialMap = makeBimap<WenzhouneseInitial, std::string>({
    {WenzhouneseInitial::B, "b"},   {WenzhouneseInitial::P, "p"},
    {WenzhouneseInitial::BB, "bb"}, {WenzhouneseInitial::M, "m"},
    {WenzhouneseInitial::F, "f"},   {WenzhouneseInitial::V, "v"},
    {WenzhouneseInitial::D, "d"},   {WenzhouneseInitial::T, "t"},
    {WenzhouneseInitial::DD, "dd"}, {WenzhouneseInitial::N, "n"},
    {WenzhouneseInitial::L, "l"},   {WenzhouneseInitial::Z, "z"},
    {WenzhouneseInitial::C, "c"},   {WenzhouneseInitial::ZZ, "zz"},
    {WenzhouneseInitial::S, "s"},   {WenzhouneseInitial::SS, "ss"},
    {WenzhouneseInitial::J, "j"},   {WenzhouneseInitial::Q, "q"},
    {WenzhouneseInitial::JJ, "jj"}, {WenzhouneseInitial::NY, "ny"},
    {WenzhouneseInitial::X, "x"},   {WenzhouneseInitial::XX, "xx"},
    {WenzhouneseInitial::G, "g"},   {WenzhouneseInitial::K, "k"},
    {WenzhouneseInitial::GG, "gg"}, {WenzhouneseInitial::NG, "ng"},
    {WenzhouneseInitial::H, "h"},   {WenzhouneseInitial::HH, "hh"},
    {WenzhouneseInitial::U, "u"},   {WenzhouneseInitial::I, "i"},
    {WenzhouneseInitial::Y, "y"},   {WenzhouneseInitial::Zero, ""},
});

static const auto finalMap = makeBimap<WenzhouneseFinal, std::string>({
    {WenzhouneseFinal::A, "a"},       {WenzhouneseFinal::AE, "ae"},
    {WenzhouneseFinal::AI, "ai"},     {WenzhouneseFinal::ANG, "ang"},
    {WenzhouneseFinal::AU, "au"},     {WenzhouneseFinal::E, "e"},
    {WenzhouneseFinal::EE, "ee"},     {WenzhouneseFinal::EI, "ei"},
    {WenzhouneseFinal::ENG, "eng"},   {WenzhouneseFinal::EU, "eu"},
    {WenzhouneseFinal::I, "i"},       {WenzhouneseFinal::IA, "ia"},
    {WenzhouneseFinal::IAE, "iae"},   {WenzhouneseFinal::IAI, "iai"},
    {WenzhouneseFinal::IANG, "iang"}, {WenzhouneseFinal::IAU, "iau"},
    {WenzhouneseFinal::IEU, "ieu"},   {WenzhouneseFinal::II, "ii"},
    {WenzhouneseFinal::IONG, "iong"}, {WenzhouneseFinal::O, "o"},
    {WenzhouneseFinal::OE, "oe"},     {WenzhouneseFinal::OEV, "oev"},
    {WenzhouneseFinal::ONG, "ong"},   {WenzhouneseFinal::U, "u"},
    {WenzhouneseFinal::UIO, "uio"},   {WenzhouneseFinal::V, "v"},
    {WenzhouneseFinal::VIO, "vio"},   {WenzhouneseFinal::VO, "vo"},
    {WenzhouneseFinal::Zero, ""},
});

static const int maxWenzhouneseLength = 6;

template <typename Iter>
std::pair<std::string_view, bool> longestMatch(Iter iter, Iter end) {
    if (std::distance(iter, end) > maxWenzhouneseLength) {
        end = iter + maxWenzhouneseLength;
    }
    auto range = std::string_view(&*iter, std::distance(iter, end));
    auto &map = getWenzhouneseMap();
    for (; range.size(); range.remove_suffix(1)) {
        auto iterPair = map.equal_range(range);
        if (iterPair.first != iterPair.second) {
            // Treat m/ng as initials so partial input can match any final.
            return std::make_pair(range, (range != "m" && range != "ng"));
        }
        if (range.size() <= 2) {
            auto iter = initialMap.right.find(std::string{range});
            if (iter != initialMap.right.end()) {
                return std::make_pair(range, false);
            }
        }
    }

    if (!range.size()) {
        range = std::string_view(&*iter, 1);
    }

    return std::make_pair(range, false);
}

std::string WenzhouneseSyllable::toString() const {
    return WenzhouneseEncoder::initialToString(initial_) +
           WenzhouneseEncoder::finalToString(final_);
}

SegmentGraph
WenzhouneseEncoder::parseUserWenzhounese(std::string userWenzhounese,
                                         bool inner) {
    SegmentGraph result(std::move(userWenzhounese));
    const auto &wenzhounese = result.data();
    auto end = wenzhounese.end();
    std::priority_queue<size_t, std::vector<size_t>, std::greater<size_t>> q;
    q.push(0);
    while (q.size()) {
        size_t top;
        do {
            top = q.top();
            q.pop();
        } while (q.size() && q.top() == top);
        if (top >= wenzhounese.size()) {
            continue;
        }
        auto iter = std::next(wenzhounese.begin(), top);
        if (*iter == '\'') {
            while (*iter == '\'' && iter != wenzhounese.end()) {
                iter++;
            }
            auto next = std::distance(wenzhounese.begin(), iter);
            result.addNext(top, next);
            if (static_cast<size_t>(next) < wenzhounese.size()) {
                q.push(next);
            }
            continue;
        }
        std::string_view str;
        bool isCompleteWenzhounese;
        std::tie(str, isCompleteWenzhounese) = longestMatch(iter, end);

        // An incomplete syllable has no alternative segmentation.
        if (!isCompleteWenzhounese) {
            result.addNext(top, top + str.size());
            q.push(top + str.size());
        } else {
            // A trailing g, i, u or v may begin the next Wenzhounese
            // syllable. Try both boundaries unless a separator fixes it.
            auto &map = getWenzhouneseMap();
            std::array<size_t, 2> nextSize;
            size_t nNextSize = 0;
            if (str.size() > 1 && top + str.size() < wenzhounese.size() &&
                wenzhounese[top + str.size()] != '\'' &&
                (str.back() == 'g' || str.back() == 'i' || str.back() == 'u' ||
                 str.back() == 'v') &&
                map.find(str.substr(0, str.size() - 1)) != map.end()) {
                // Compare the following syllable at each possible boundary.
                auto nextMatch = longestMatch(iter + str.size(), end);
                auto nextMatchAlt = longestMatch(iter + str.size() - 1, end);
                auto matchSize = str.size() + nextMatch.first.size();
                auto matchSizeAlt = str.size() - 1 + nextMatchAlt.first.size();
                if (std::make_pair(matchSize, nextMatch.second) >=
                    std::make_pair(matchSizeAlt, nextMatchAlt.second)) {
                    result.addNext(top, top + str.size());
                    q.push(top + str.size());
                    nextSize[nNextSize++] = str.size();
                }
                if (std::make_pair(matchSize, nextMatch.second) <=
                    std::make_pair(matchSizeAlt, nextMatchAlt.second)) {
                    result.addNext(top, top + str.size() - 1);
                    q.push(top + str.size() - 1);
                    nextSize[nNextSize++] = str.size() - 1;
                }
            } else {
                result.addNext(top, top + str.size());
                q.push(top + str.size());
                nextSize[nNextSize++] = str.size();
            }

            for (size_t i = 0; i < nNextSize; i++) {
                if (nextSize[i] >= 4 && inner) {
                    auto &innerSegments = getInnerSegment();
                    auto iter = innerSegments.find(
                        std::string{str.substr(0, nextSize[i])});
                    if (iter != innerSegments.end()) {
                        result.addNext(top, top + iter->second.first.size());
                        result.addNext(top + iter->second.first.size(),
                                       top + nextSize[i]);
                    }
                }
            }
        }
    }
    return result;
}

std::vector<char>
WenzhouneseEncoder::encodeOneUserWenzhounese(std::string wenzhounese) {
    if (wenzhounese.empty()) {
        return {};
    }
    auto graph = parseUserWenzhounese(std::move(wenzhounese), false);
    std::vector<char> result;
    const SegmentGraphNode *node = &graph.start(), *prev = nullptr;
    while (node->nextSize()) {
        prev = node;
        node = &node->nexts().front();
        auto seg = graph.segment(*prev, *node);
        if (seg.empty() || seg[0] == '\'') {
            continue;
        }
        auto syls = stringToSyllables(seg);
        if (!syls.size()) {
            return {};
        }
        result.push_back(static_cast<char>(syls[0].first));
        result.push_back(static_cast<char>(syls[0].second[0].first));
    }
    return result;
}

bool WenzhouneseEncoder::isValidUserWenzhounese(const char *data, size_t size) {
    if (size % 2 != 0) {
        return false;
    }

    for (size_t i = 0; i < size / 2; i++) {
        if (!WenzhouneseEncoder::isValidInitial(data[i * 2])) {
            return false;
        }
    }
    return true;
}

std::string WenzhouneseEncoder::decodeFullWenzhounese(const char *data,
                                                      size_t size) {
    if (size % 2 != 0) {
        throw std::invalid_argument("invalid wenzhounese key");
    }
    std::string result;
    for (size_t i = 0, e = size / 2; i < e; i++) {
        if (i) {
            result += '\'';
        }
        result += initialToString(static_cast<WenzhouneseInitial>(data[i * 2]));
        result += finalToString(static_cast<WenzhouneseFinal>(data[i * 2 + 1]));
    }
    return result;
}

const std::string &
WenzhouneseEncoder::initialToString(WenzhouneseInitial initial) {
    const static std::vector<std::string> s = []() {
        std::vector<std::string> s;
        s.resize(lastInitial - firstInitial + 1);
        for (char c = firstInitial; c <= lastInitial; c++) {
            auto iter =
                initialMap.left.find(static_cast<WenzhouneseInitial>(c));
            s[c - firstInitial] = iter->second;
        }
        return s;
    }();
    auto c = static_cast<char>(initial);
    if (c >= firstInitial && c <= lastInitial) {
        return s[c - firstInitial];
    }
    return emptyString;
}

WenzhouneseInitial WenzhouneseEncoder::stringToInitial(const std::string &str) {
    auto iter = initialMap.right.find(str);
    if (iter != initialMap.right.end()) {
        return iter->second;
    }
    return WenzhouneseInitial::Invalid;
}

const std::string &WenzhouneseEncoder::finalToString(WenzhouneseFinal final) {
    const static std::vector<std::string> s = []() {
        std::vector<std::string> s;
        s.resize(lastFinal - firstFinal + 1);
        for (char c = firstFinal; c <= lastFinal; c++) {
            auto iter = finalMap.left.find(static_cast<WenzhouneseFinal>(c));
            s[c - firstFinal] = iter->second;
        }
        return s;
    }();
    auto c = static_cast<char>(final);
    if (c >= firstFinal && c <= lastFinal) {
        return s[c - firstFinal];
    }
    return emptyString;
}

WenzhouneseFinal WenzhouneseEncoder::stringToFinal(const std::string &str) {
    auto iter = finalMap.right.find(str);
    if (iter != finalMap.right.end()) {
        return iter->second;
    }
    return WenzhouneseFinal::Invalid;
}

bool WenzhouneseEncoder::isValidInitialFinal(WenzhouneseInitial initial,
                                             WenzhouneseFinal final) {
    if (initial != WenzhouneseInitial::Invalid &&
        final != WenzhouneseFinal::Invalid) {
        int16_t encode =
            ((static_cast<int16_t>(initial) -
              WenzhouneseEncoder::firstInitial) *
             (WenzhouneseEncoder::lastFinal - WenzhouneseEncoder::firstFinal +
              1)) +
            (static_cast<int16_t>(final) - WenzhouneseEncoder::firstFinal);
        const auto &a = getEncodedInitialFinal();
        return encode < static_cast<int>(a.size()) && a[encode];
    }
    return false;
}

static void
getFuzzy(std::vector<std::pair<WenzhouneseInitial,
                               std::vector<std::pair<WenzhouneseFinal, bool>>>>
             &syls,
         WenzhouneseSyllable syl) {
    WenzhouneseInitial initials[2] = {syl.initial(),
                                      WenzhouneseInitial::Invalid};
    WenzhouneseFinal finals[2] = {syl.final(), WenzhouneseFinal::Invalid};
    int initialSize = 1;
    int finalSize = 1;

    for (int i = 0; i < initialSize; i++) {
        for (int j = 0; j < finalSize; j++) {
            auto initial = initials[i];
            auto final = finals[j];
            if ((i == 0 && j == 0) || final == WenzhouneseFinal::Invalid ||
                WenzhouneseEncoder::isValidInitialFinal(initial, final)) {
                auto iter = std::find_if(
                    syls.begin(), syls.end(),
                    [initial](const auto &p) { return p.first == initial; });
                if (iter == syls.end()) {
                    syls.emplace_back(std::piecewise_construct,
                                      std::forward_as_tuple(initial),
                                      std::forward_as_tuple());
                    iter = std::prev(syls.end());
                }
                auto &finals = iter->second;
                if (std::find_if(finals.begin(), finals.end(),
                                 [final](auto &p) {
                                     return p.first == final;
                                 }) == finals.end()) {
                    finals.emplace_back(final, i > 0 || j > 0);
                }
            }
        }
    }
}

MatchedWenzhouneseSyllables
WenzhouneseEncoder::stringToSyllables(std::string_view wenzhounese) {
    std::vector<std::pair<WenzhouneseInitial,
                          std::vector<std::pair<WenzhouneseFinal, bool>>>>
        result;
    auto &map = getWenzhouneseMap();
    // Use an unspecified final for m/ng so these initials match all finals.
    if (wenzhounese != "m" && wenzhounese != "ng") {
        auto iterPair = map.equal_range(wenzhounese);
        for (auto &item :
             boost::make_iterator_range(iterPair.first, iterPair.second)) {
            getFuzzy(result, {item.initial(), item.final()});
        }
    }

    auto iter = initialMap.right.find(std::string{wenzhounese});
    if (initialMap.right.end() != iter) {
        getFuzzy(result, {iter->second, WenzhouneseFinal::Invalid});
    }

    if (result.size() == 0) {
        result.emplace_back(
            std::piecewise_construct,
            std::forward_as_tuple(WenzhouneseInitial::Invalid),
            std::forward_as_tuple(
                1, std::make_pair(WenzhouneseFinal::Invalid, false)));
    }

    return result;
}

std::vector<char>
WenzhouneseEncoder::encodeFullWenzhounese(std::string_view wenzhounese) {
    std::vector<std::string> syllables;
    boost::split(syllables, wenzhounese, boost::is_any_of("'"));
    std::vector<char> result;
    result.resize(syllables.size() * 2);
    int idx = 0;
    for (const auto &syllable : syllables) {
        auto &map = getWenzhouneseMap();
        auto iter = map.find(syllable);
        if (iter == map.end()) {
            throw std::invalid_argument("invalid full wenzhounese: " +
                                        std::string{wenzhounese});
        }
        result[idx++] = static_cast<char>(iter->initial());
        result[idx++] = static_cast<char>(iter->final());
    }

    return result;
}

} // namespace wenzhounese
} // namespace libime
