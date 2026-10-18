/*
 * SPDX-FileCopyrightText: 2018~2018 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _LIBIME_WENZHOUNESE_WENZHOUNESECONTEXT_H_
#define _LIBIME_WENZHOUNESE_WENZHOUNESECONTEXT_H_

#include "libimewenzhounese_export.h"
#include <libime/core/inputbuffer.h>
#include <libime/core/lattice.h>
#include <libime/wenzhounese/wenzhouneseime.h>
#include <vector>

namespace libime {
namespace wenzhounese {

class WenzhouneseContextPrivate;

class LIBIMEWENZHOUNESE_EXPORT WenzhouneseContext : public InputBuffer {
public:
    WenzhouneseContext(WenzhouneseIME *ime);
    virtual ~WenzhouneseContext();

    void erase(size_t from, size_t to) override;
    void setCursor(size_t pos) override;

    const std::vector<SentenceResult> &candidates() const;
    void select(size_t idx);
    void cancel();
    bool cancelTill(size_t pos);

    bool selected() const;
    std::string sentence() const {
        auto &c = candidates();
        if (c.size()) {
            return selectedSentence() + c[0].toString();
        } else {
            return selectedSentence();
        }
    }

    std::string preedit() const;
    std::pair<std::string, size_t> preeditWithCursor() const;
    std::string selectedSentence() const;
    size_t selectedLength() const;

    std::vector<std::string> selectedWords() const;

    std::string selectedFullWenzhounese() const;
    std::string candidateFullWenzhounese(size_t i) const;

    void learn();

    int wenzhouneseBeforeCursor() const;
    int wenzhouneseAfterCursor() const;

    WenzhouneseIME *ime() const;

    State state() const;

protected:
    bool typeImpl(const char *s, size_t length) override;

private:
    void update();
    bool learnWord();
    std::unique_ptr<WenzhouneseContextPrivate> d_ptr;
    FCITX_DECLARE_PRIVATE(WenzhouneseContext);
};

} // namespace wenzhounese
} // namespace libime

#endif // _LIBIME_WENZHOUNESE_WENZHOUNESECONTEXT_H_
