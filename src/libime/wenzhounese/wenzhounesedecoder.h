/*
 * SPDX-FileCopyrightText: 2018~2018 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _LIBIME_WENZHOUNESE_WENZHOUNESEDECODER_H_
#define _LIBIME_WENZHOUNESE_WENZHOUNESEDECODER_H_

#include "libimewenzhounese_export.h"
#include <libime/core/decoder.h>
#include <libime/wenzhounese/wenzhounesedictionary.h>

namespace libime {
namespace wenzhounese {

class WenzhouneseLatticeNodePrivate;

class LIBIMEWENZHOUNESE_EXPORT WenzhouneseLatticeNode : public LatticeNode {
public:
    WenzhouneseLatticeNode(std::string_view word, WordIndex idx,
                           SegmentGraphPath path, const State &state,
                           float cost,
                           std::unique_ptr<WenzhouneseLatticeNodePrivate> data);
    virtual ~WenzhouneseLatticeNode();

    const std::string &encodedWenzhounese() const;

private:
    std::unique_ptr<WenzhouneseLatticeNodePrivate> d_ptr;
};

class LIBIMEWENZHOUNESE_EXPORT WenzhouneseDecoder : public Decoder {
public:
    WenzhouneseDecoder(const WenzhouneseDictionary *dict,
                       const LanguageModelBase *model)
        : Decoder(dict, model) {}

protected:
    LatticeNode *createLatticeNodeImpl(const SegmentGraphBase &graph,
                                       const LanguageModelBase *model,
                                       std::string_view word, WordIndex idx,
                                       SegmentGraphPath path,
                                       const State &state, float cost,
                                       std::unique_ptr<LatticeNodeData> data,
                                       bool onlyPath) const override;
};
} // namespace wenzhounese
} // namespace libime

#endif // _LIBIME_WENZHOUNESE_WENZHOUNESEDECODER_H_
