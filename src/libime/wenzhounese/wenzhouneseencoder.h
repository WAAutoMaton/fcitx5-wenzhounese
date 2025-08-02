/*
 * SPDX-FileCopyrightText: 2017~2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _LIBIME_WENZHOUNESE_WENZHOUNESEENCODER_H_
#define _LIBIME_WENZHOUNESE_WENZHOUNESEENCODER_H_

#include "libimewenzhounese_export.h"
#include <libime/core/segmentgraph.h>

namespace libime {
namespace wenzhounese {

enum class WenzhouneseInitial : char {
    Invalid = 0,
    B,
    P,
    BB,
    M,
    F,
    V,
    D,
    T,
    DD,
    N,
    L,
    Z,
    C,
    ZZ,
    S,
    SS,
    J,
    Q,
    JJ,
    NY,
    X,
    XX,
    G,
    K,
    GG,
    NG,
    H,
    HH,
    U,
    //V,
    I,
    Y,
    Zero,
};

enum class WenzhouneseFinal : char {
    Invalid = 0, 
    A,
    AE,
    AI,
    ANG,
    AU,
    E,
    EE,
    EI,
    ENG,
    EU,
    I,
    IA,
    IAE,
    IAI,
    IANG,
    IAU,
    IEU,
    II,
    IONG,
    O,
    OE,
    OEV,
    ONG,
    U,
    UIO,
    V,
    VIO,
    VO,
    Zero,
};

struct LIBIMEWENZHOUNESE_EXPORT WenzhouneseSyllable {
public:
    WenzhouneseSyllable(WenzhouneseInitial initial, WenzhouneseFinal final)
        : initial_(initial), final_(final) {}
    FCITX_INLINE_DEFINE_DEFAULT_DTOR_AND_COPY(WenzhouneseSyllable)

    WenzhouneseInitial initial() const { return initial_; }
    WenzhouneseFinal final() const { return final_; }

    std::string toString() const;

    bool operator==(const WenzhouneseSyllable &other) const {
        return initial_ == other.initial_ && final_ == other.final_;
    }

    bool operator!=(const WenzhouneseSyllable &other) const {
        return !(*this == other);
    }
    bool operator<(const WenzhouneseSyllable &other) const {
        return std::make_pair(initial_, final_) <
               std::make_pair(other.initial_, other.final_);
    }
    bool operator<=(const WenzhouneseSyllable &other) const {
        return *this < other || *this == other;
    }
    bool operator>(const WenzhouneseSyllable &other) const {
        return !(*this <= other);
    }
    bool operator>=(const WenzhouneseSyllable &other) const {
        return !(*this < other);
    }

private:
    WenzhouneseInitial initial_;
    WenzhouneseFinal final_;
};

using MatchedWenzhouneseSyllables = std::vector<
    std::pair<WenzhouneseInitial, std::vector<std::pair<WenzhouneseFinal, bool>>>>;

class LIBIMEWENZHOUNESE_EXPORT WenzhouneseEncoder {
public:
    static SegmentGraph parseUserWenzhounese(std::string wenzhounese,
                                          bool inner = true);
    static std::vector<char> encodeOneUserWenzhounese(std::string wenzhounese);
    static bool isValidUserWenzhounese(const char *data, size_t size);

    static std::vector<char> encodeFullWenzhounese(std::string_view wenzhounese);

    static std::string decodeFullWenzhounese(const char *data, size_t size);
    static std::string decodeFullWenzhounese(std::string_view data) {
        return decodeFullWenzhounese(data.data(), data.size());
    }

    static MatchedWenzhouneseSyllables
    stringToSyllables(std::string_view wenzhounese);

    static const std::string &initialToString(WenzhouneseInitial initial);
    static WenzhouneseInitial stringToInitial(const std::string &str);
    static bool isValidInitial(char c) {
        return c >= firstInitial && c <= lastInitial;
    }

    static const std::string &finalToString(WenzhouneseFinal final);
    static WenzhouneseFinal stringToFinal(const std::string &str);
    static bool isValidFinal(char c) {
        return c >= firstFinal && c <= lastFinal;
    }

    static bool isValidInitialFinal(WenzhouneseInitial initial,
                                    WenzhouneseFinal final);

    static const char firstInitial = static_cast<char>(WenzhouneseInitial::B);
    static const char lastInitial = static_cast<char>(WenzhouneseInitial::Zero);
    static const char firstFinal = static_cast<char>(WenzhouneseFinal::A);
    static const char lastFinal = static_cast<char>(WenzhouneseFinal::Zero);
};

} // namespace wenzhounese
} // namespace libime
#endif // _LIBIME_WENZHOUNESE_WENZHOUNESEENCODER_H_

