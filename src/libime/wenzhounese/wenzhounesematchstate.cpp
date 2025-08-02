/*
 * SPDX-FileCopyrightText: 2018~2018 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include "wenzhounesecontext.h"
#include "wenzhouneseime.h"
#include "wenzhounesematchstate_p.h"

namespace libime {
namespace wenzhounese {

WenzhouneseMatchState::WenzhouneseMatchState(WenzhouneseContext *context)
    : d_ptr(std::make_unique<WenzhouneseMatchStatePrivate>(context)) {}
WenzhouneseMatchState::~WenzhouneseMatchState() {}

void WenzhouneseMatchState::clear() {
    FCITX_D();
    d->matchedPaths_.clear();
    d->nodeCacheMap_.clear();
    d->matchCacheMap_.clear();
}

void WenzhouneseMatchState::discardNode(
    const std::unordered_set<const SegmentGraphNode *> &nodes) {
    FCITX_D();
    for (auto node : nodes) {
        d->matchedPaths_.erase(node);
    }
    for (auto &p : d->matchedPaths_) {
        auto &l = p.second;
        auto iter = l.begin();
        while (iter != l.end()) {
            if (nodes.count(iter->path_.front())) {
                iter = l.erase(iter);
            } else {
                iter++;
            }
        }
    }
}

void WenzhouneseMatchState::discardDictionary(size_t idx) {
    FCITX_D();
    d->matchCacheMap_.erase(d->context_->ime()->dict()->trie(idx));
    d->nodeCacheMap_.erase(d->context_->ime()->dict()->trie(idx));
}

} // namespace wenzhounese
} // namespace libime
