/*
 * SPDX-FileCopyrightText: 2018~2018 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include "wenzhounesedecoder_p.h"
#include <cmath>

namespace libime {
namespace wenzhounese {

WenzhouneseLatticeNode::WenzhouneseLatticeNode(
    std::string_view word, WordIndex idx, SegmentGraphPath path,
    const State &state, float cost,
    std::unique_ptr<WenzhouneseLatticeNodePrivate> data)
    : LatticeNode(word, idx, std::move(path), state, cost),
      d_ptr(std::move(data)) {}

WenzhouneseLatticeNode::~WenzhouneseLatticeNode() = default;

const std::string &WenzhouneseLatticeNode::encodedWenzhounese() const {
    static const std::string empty;
    if (!d_ptr) {
        return empty;
    }
    return d_ptr->encodedWenzhounese_;
}

LatticeNode *WenzhouneseDecoder::createLatticeNodeImpl(
    const SegmentGraphBase &graph, const LanguageModelBase *model,
    std::string_view word, WordIndex idx, SegmentGraphPath path,
    const State &state, float cost, std::unique_ptr<LatticeNodeData> data,
    bool onlyPath) const {
    std::unique_ptr<WenzhouneseLatticeNodePrivate> wenzhouneseData(
        static_cast<WenzhouneseLatticeNodePrivate *>(data.release()));
    if (model->isUnknown(idx, word)) {
        // we don't really care about a lot of unknown single character
        // which is not used for candidates
        if ((wenzhouneseData &&
             wenzhouneseData->encodedWenzhounese_.size() == 2) &&
            path.front() != &graph.start() && !onlyPath) {
            return nullptr;
        }
    }

    return new WenzhouneseLatticeNode(word, idx, std::move(path), state, cost,
                                      std::move(wenzhouneseData));
}

} // namespace wenzhounese
} // namespace libime
