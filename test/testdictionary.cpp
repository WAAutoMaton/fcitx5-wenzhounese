/*
 * SPDX-FileCopyrightText: 2018~2018 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

/*
 * SPDX-FileCopyrightText: 2017~2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "libime/wenzhounese/wenzhounesedictionary.h"
#include "libime/wenzhounese/wenzhouneseencoder.h"
#include "testdir.h"
#include <fcitx-utils/log.h>

int main() {
    using namespace libime;
    using namespace libime::wenzhounese;
    WenzhouneseDictionary dict;
    dict.load(WenzhouneseDictionary::SystemDict,
              LIBIME_BINARY_DIR "/data/wenzhounese.dict",
              WenzhouneseDictFormat::Binary);

    char c[] = {static_cast<char>(WenzhouneseInitial::J),
                static_cast<char>(WenzhouneseFinal::IN),
                static_cast<char>(WenzhouneseInitial::H),
                static_cast<char>(WenzhouneseFinal::AU)};
    dict.matchWords(c, 2,
                    [c](std::string_view encodedWenzhounese,
                        std::string_view hanzi, float cost) {
                        std::cout << WenzhouneseEncoder::decodeFullWenzhounese(
                                         encodedWenzhounese)
                                  << " " << hanzi << " " << cost << std::endl;
                        return true;
                    });
    dict.matchWords(c + 2, 2,
                    [c](std::string_view encodedWenzhounese,
                        std::string_view hanzi, float cost) {
                        std::cout << WenzhouneseEncoder::decodeFullWenzhounese(
                                         encodedWenzhounese)
                                  << " " << hanzi << " " << cost << std::endl;
                        return true;
                    });

    auto graph = WenzhouneseEncoder::parseUserWenzhounese("jinhau", true);
    dict.matchPrefix(graph,
                     [&graph](const SegmentGraphPath &path, WordNode &node,
                              float, std::unique_ptr<LatticeNodeData>) {
                         for (auto &step : path) {
                             std::cout << step->index() << " ";
                         }
                         std::cout << node.word() << std::endl;
                         return true;
                     });

    dict.save(0, LIBIME_BINARY_DIR "/test/testwenzhounesedictionary.dict",
              WenzhouneseDictFormat::Binary);
    // dict.save(0, std::cout, WenzhouneseDictFormat::Text);
    return 0;
}
