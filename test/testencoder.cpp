/*
 * SPDX-FileCopyrightText: 2017~2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "libime/wenzhounese/wenzhounesedata.h"
#include "libime/wenzhounese/wenzhouneseencoder.h"
#include <fcitx-utils/log.h>

using namespace libime;
using namespace libime::wenzhounese;

void dfs(const SegmentGraph &segs) {
    FCITX_ASSERT(segs.checkGraph());

    auto callback = [](const SegmentGraphBase &segs,
                       const std::vector<size_t> &path) {
        size_t s = 0;
        for (auto e : path) {
            std::cout << segs.segment(s, e) << " ";
            s = e;
        }
        std::cout << std::endl;
        return true;
    };

    segs.dfs(callback);
}

int main() {
    std::unordered_set<std::string> seen;
    for (auto &p : getWenzhouneseMap()) {
        auto wenzhounese = p.wenzhounese();
        auto initial = p.initial();
        auto final = p.final();

        if (p.fuzzy()) {
            continue;
        }

        auto fullWenzhounese = WenzhouneseEncoder::initialToString(initial) +
                            WenzhouneseEncoder::finalToString(final);
        // make sure valid item is unique
        auto result = seen.insert(wenzhounese);
        FCITX_ASSERT(result.second);

        int16_t encode =
            ((static_cast<int16_t>(initial) - WenzhouneseEncoder::firstInitial) *
             (WenzhouneseEncoder::lastFinal - WenzhouneseEncoder::firstFinal + 1)) +
            (static_cast<int16_t>(final) - WenzhouneseEncoder::firstFinal);
        FCITX_ASSERT(WenzhouneseEncoder::isValidInitialFinal(initial, final))
            << " " << encode;
        FCITX_ASSERT(fullWenzhounese == wenzhounese)
            << " " << fullWenzhounese << " " << wenzhounese;
        std::cout << encode << "," << std::endl;
    }

    dfs(WenzhouneseEncoder::parseUserWenzhounese("sangwut"));
    dfs(WenzhouneseEncoder::parseUserWenzhounese("ngng"));
    dfs(WenzhouneseEncoder::parseUserWenzhounese("ngaat"));
    dfs(WenzhouneseEncoder::parseUserWenzhounese("ngngaat"));
    dfs(WenzhouneseEncoder::parseUserWenzhounese("neizaudaajatgeoi"));
    dfs(WenzhouneseEncoder::parseUserWenzhounese("jinhauonjathaa"));
    dfs(WenzhouneseEncoder::parseUserWenzhounese("jinha"));
    dfs(WenzhouneseEncoder::parseUserWenzhounese("jinhau"));

    return 0;
}
