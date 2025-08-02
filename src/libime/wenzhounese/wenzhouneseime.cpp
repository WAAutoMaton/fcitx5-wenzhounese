/*
 * SPDX-FileCopyrightText: 2018~2018 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */

#include "wenzhouneseime.h"
#include "wenzhounesedecoder.h"
#include "libime/core/userlanguagemodel.h"

namespace libime {
namespace wenzhounese {

class WenzhouneseIMEPrivate : fcitx::QPtrHolder<WenzhouneseIME> {
public:
    WenzhouneseIMEPrivate(WenzhouneseIME *q, std::unique_ptr<WenzhouneseDictionary> dict,
                       std::unique_ptr<UserLanguageModel> model)
        : fcitx::QPtrHolder<WenzhouneseIME>(q), dict_(std::move(dict)),
          model_(std::move(model)), decoder_(std::make_unique<WenzhouneseDecoder>(
                                        dict_.get(), model_.get())) {}

    FCITX_DEFINE_SIGNAL_PRIVATE(WenzhouneseIME, optionChanged);

    std::unique_ptr<WenzhouneseDictionary> dict_;
    std::unique_ptr<UserLanguageModel> model_;
    std::unique_ptr<WenzhouneseDecoder> decoder_;
    bool innerSegment_ = true;
    size_t nbest_ = 1;
    size_t beamSize_ = Decoder::beamSizeDefault;
    size_t frameSize_ = Decoder::frameSizeDefault;
    float maxDistance_ = std::numeric_limits<float>::max();
    float minPath_ = -std::numeric_limits<float>::max();
};

WenzhouneseIME::WenzhouneseIME(std::unique_ptr<WenzhouneseDictionary> dict,
                         std::unique_ptr<UserLanguageModel> model)
    : d_ptr(std::make_unique<WenzhouneseIMEPrivate>(this, std::move(dict),
                                                 std::move(model))) {}

WenzhouneseIME::~WenzhouneseIME() {}

bool WenzhouneseIME::innerSegment() const {
    FCITX_D();
    return d->innerSegment_;
}

void WenzhouneseIME::setInnerSegment(bool inner) {
    FCITX_D();
    d->innerSegment_ = inner;
    emit<WenzhouneseIME::optionChanged>();
}

WenzhouneseDictionary *WenzhouneseIME::dict() {
    FCITX_D();
    return d->dict_.get();
}

const WenzhouneseDictionary *WenzhouneseIME::dict() const {
    FCITX_D();
    return d->dict_.get();
}

const WenzhouneseDecoder *WenzhouneseIME::decoder() const {
    FCITX_D();
    return d->decoder_.get();
}

UserLanguageModel *WenzhouneseIME::model() {
    FCITX_D();
    return d->model_.get();
}

const UserLanguageModel *WenzhouneseIME::model() const {
    FCITX_D();
    return d->model_.get();
}

size_t WenzhouneseIME::nbest() const {
    FCITX_D();
    return d->nbest_;
}

void WenzhouneseIME::setNBest(size_t n) {
    FCITX_D();
    if (d->nbest_ != n) {
        d->nbest_ = n;
        emit<WenzhouneseIME::optionChanged>();
    }
}

size_t WenzhouneseIME::beamSize() const {
    FCITX_D();
    return d->beamSize_;
}

void WenzhouneseIME::setBeamSize(size_t n) {
    FCITX_D();
    if (d->beamSize_ != n) {
        d->beamSize_ = n;
        emit<WenzhouneseIME::optionChanged>();
    }
}

size_t WenzhouneseIME::frameSize() const {
    FCITX_D();
    return d->frameSize_;
}

void WenzhouneseIME::setFrameSize(size_t n) {
    FCITX_D();
    if (d->frameSize_ != n) {
        d->frameSize_ = n;
        emit<WenzhouneseIME::optionChanged>();
    }
}

void WenzhouneseIME::setScoreFilter(float maxDistance, float minPath) {
    FCITX_D();
    if (d->maxDistance_ != maxDistance || d->minPath_ != minPath) {
        d->maxDistance_ = maxDistance;
        d->minPath_ = minPath;
        emit<WenzhouneseIME::optionChanged>();
    }
}

float WenzhouneseIME::maxDistance() const {
    FCITX_D();
    return d->maxDistance_;
}

float WenzhouneseIME::minPath() const {
    FCITX_D();
    return d->minPath_;
}
} // namespace wenzhounese
} // namespace libime
