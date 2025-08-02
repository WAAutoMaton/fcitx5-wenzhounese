/*
 * SPDX-FileCopyrightText: 2018~2018 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEMATCHSTATE_H_
#define _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEMATCHSTATE_H_

#include "libimewenzhounese_export.h"
#include <fcitx-utils/macros.h>
#include <libime/wenzhounese/wenzhouneseencoder.h>
#include <unordered_set>

namespace libime {

class SegmentGraphNode;

namespace wenzhounese {

class WenzhouneseMatchStatePrivate;
class WenzhouneseContext;

// Provides caching mechanism used by WenzhouneseContext.
class LIBIMEWENZHOUNESE_EXPORT WenzhouneseMatchState {
    friend class WenzhouneseMatchContext;

public:
    WenzhouneseMatchState(WenzhouneseContext *context);
    ~WenzhouneseMatchState();

    // Invalidate everything in the state.
    void clear();

    // Invalidate a set of node, usually caused by the change of user input.
    void discardNode(const std::unordered_set<const SegmentGraphNode *> &node);

    // Invalidate a whole dictionary, usually caused by the change to the
    // dictionary.
    void discardDictionary(size_t idx);

private:
    std::unique_ptr<WenzhouneseMatchStatePrivate> d_ptr;
    FCITX_DECLARE_PRIVATE(WenzhouneseMatchState);
};

} // namespace wenzhounese
} // namespace libime

#endif // _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEMATCHSTATE_H_
