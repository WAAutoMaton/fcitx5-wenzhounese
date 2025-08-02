/*
 * SPDX-FileCopyrightText: 2018~2018 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEIME_H_
#define _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEIME_H_

#include "libimewenzhounese_export.h"
#include <fcitx-utils/connectableobject.h>
#include <fcitx-utils/macros.h>
#include <libime/wenzhounese/wenzhouneseencoder.h>
#include <limits>
#include <memory>

namespace libime {

class UserLanguageModel;

namespace wenzhounese {

class WenzhouneseIMEPrivate;
class WenzhouneseDecoder;
class WenzhouneseDictionary;

/// \brief Provides shared data for WenzhouneseContext.
class LIBIMEWENZHOUNESE_EXPORT WenzhouneseIME : public fcitx::ConnectableObject {
public:
    WenzhouneseIME(std::unique_ptr<WenzhouneseDictionary> dict,
                std::unique_ptr<UserLanguageModel> model);
    virtual ~WenzhouneseIME();

    bool innerSegment() const;
    void setInnerSegment(bool inner);
    size_t nbest() const;
    void setNBest(size_t n);
    size_t beamSize() const;
    void setBeamSize(size_t n);
    size_t frameSize() const;
    void setFrameSize(size_t n);
    void setScoreFilter(float maxDistance = std::numeric_limits<float>::max(),
                        float minPath = -std::numeric_limits<float>::max());

    float maxDistance() const;
    float minPath() const;

    WenzhouneseDictionary *dict();
    const WenzhouneseDictionary *dict() const;
    const WenzhouneseDecoder *decoder() const;
    UserLanguageModel *model();
    const UserLanguageModel *model() const;

    FCITX_DECLARE_SIGNAL(WenzhouneseIME, optionChanged, void());

private:
    std::unique_ptr<WenzhouneseIMEPrivate> d_ptr;
    FCITX_DECLARE_PRIVATE(WenzhouneseIME);
};

} // namespace wenzhounese
} // namespace libime

#endif // _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEIME_H_
