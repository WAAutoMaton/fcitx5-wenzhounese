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
#include "libime/core/languagemodel.h"
#include "libime/wenzhounese/wenzhounesedecoder.h"
#include "libime/wenzhounese/wenzhounesedictionary.h"
#include "libime/wenzhounese/wenzhouneseencoder.h"
#include "testdir.h"
#include <fcitx-utils/log.h>
#include <limits>

using namespace libime;
using namespace libime::wenzhounese;

void testTime(WenzhouneseDictionary &, Decoder &decoder,
              const char *wenzhounese, int nbest = 1) {
    auto graph = WenzhouneseEncoder::parseUserWenzhounese(wenzhounese, true);
    Lattice lattice;
    decoder.decode(lattice, graph, nbest, decoder.model()->nullState(),
                   std::numeric_limits<float>::max(),
                   -std::numeric_limits<float>::max(), Decoder::beamSizeDefault,
                   Decoder::frameSizeDefault, nullptr);
    for (size_t i = 0, e = lattice.sentenceSize(); i < e; i++) {
        auto &sentence = lattice.sentence(i);
        for (auto &p : sentence.sentence()) {
            std::cout << p->word() << " ";
        }
        std::cout << sentence.score() << std::endl;
    }
}

int main() {
    WenzhouneseDictionary dict;
    dict.load(WenzhouneseDictionary::SystemDict,
              LIBIME_BINARY_DIR "/data/wenzhounese.dict",
              WenzhouneseDictFormat::Binary);
    LanguageModel model(LIBIME_BINARY_DIR "/data/zh_HK.lm");
    WenzhouneseDecoder decoder(&dict, &model);
    testTime(dict, decoder, "jinhau");

    return 0;
}
