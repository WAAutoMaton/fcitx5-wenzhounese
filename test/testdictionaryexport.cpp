/*
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "libime/wenzhounese/wenzhounesedictionary.h"
#include <fcitx-utils/log.h>
#include <map>
#include <sstream>
#include <string>
#include <utility>

using libime::wenzhounese::WenzhouneseDictFormat;
using libime::wenzhounese::WenzhouneseDictionary;

using Entries = std::map<std::pair<std::string, std::string>, float>;

std::string saveText(WenzhouneseDictionary &dictionary, size_t index) {
    std::ostringstream output;
    dictionary.save(index, output, WenzhouneseDictFormat::Text);
    return output.str();
}

void checkEntries(const std::string &text, const Entries &expected) {
    std::istringstream input(text);
    Entries actual;
    std::string word;
    std::string spelling;
    float cost;
    while (input >> word >> spelling >> cost) {
        FCITX_ASSERT(
            actual.emplace(std::make_pair(word, spelling), cost).second);
    }
    FCITX_ASSERT(input.eof());
    FCITX_ASSERT(actual == expected);
}

int main() {
    // B initials and A finals both encode to the dictionary separator byte.
    const Entries expected = {
        {{"\u516b", "ba"}, 0.0f},
        {{"\u5df4", "ba"}, -0.5f},
        {{"\u767d", "bai"}, -1.25f},
        {{"\u62ff", "na"}, -2.5f},
        {{"\u554a", "a"}, -3.75f},
        {{"\u4f60", "niae"}, -4.0f},
        {{"\u4f60\u516b", "niae'ba"}, -5.5f},
        {{"\u62ff\u4f60", "na'niae"}, -6.25f},
        {{"\u516b\u62ff\u516b", "ba'na'ba"}, -7.0f},
        {{"ascii", "niae'ba'niae"}, -8.5f},
    };
    std::ostringstream source;
    for (const auto &[key, cost] : expected) {
        source << key.first << ' ' << key.second << ' ' << cost << '\n';
    }

    for (const auto index :
         {WenzhouneseDictionary::SystemDict, WenzhouneseDictionary::UserDict}) {
        WenzhouneseDictionary dictionary;
        std::istringstream input(source.str());
        dictionary.load(index, input, WenzhouneseDictFormat::Text);
        const auto text = saveText(dictionary, index);
        checkEntries(text, expected);

        std::stringstream binary(std::ios::in | std::ios::out |
                                 std::ios::binary);
        dictionary.save(index, binary, WenzhouneseDictFormat::Binary);
        WenzhouneseDictionary restored;
        restored.load(index, binary, WenzhouneseDictFormat::Binary);
        FCITX_ASSERT(saveText(restored, index) == text);

        std::istringstream exported(text);
        WenzhouneseDictionary reimported;
        reimported.load(index, exported, WenzhouneseDictFormat::Text);
        FCITX_ASSERT(saveText(reimported, index) == text);

        WenzhouneseDictionary added;
        for (const auto &[key, cost] : expected) {
            added.addWord(index, key.second, key.first, cost);
        }
        checkEntries(saveText(added, index), expected);
    }
    return 0;
}
