/*
 * SPDX-FileCopyrightText: 2018~2018 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEDECODER_P_H_
#define _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEDECODER_P_H_

#include "wenzhounesedecoder.h"
#include <libime/core/lattice.h>

namespace libime {
namespace wenzhounese {

class WenzhouneseLatticeNodePrivate : public LatticeNodeData {
public:
    WenzhouneseLatticeNodePrivate(std::string_view encodedWenzhounese)
        : encodedWenzhounese_(encodedWenzhounese) {}

    std::string encodedWenzhounese_;
};

} // namespace wenzhounese
} // namespace libime

#endif // _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEDECODER_P_H_
