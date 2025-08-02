/*
 * SPDX-FileCopyrightText: 2018~2018 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEDICTIONARY_H_
#define _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEDICTIONARY_H_

#include "libimewenzhounese_export.h"
#include <libime/core/triedictionary.h>

namespace libime {
namespace wenzhounese {

enum class WenzhouneseDictFormat { Text, Binary };

class WenzhouneseDictionaryPrivate;

typedef std::function<bool(std::string_view encodedWenzhounese,
                           std::string_view hanzi, float cost)>
    WenzhouneseMatchCallback;
class WenzhouneseDictionary;

using WenzhouneseTrie = typename TrieDictionary::TrieType;

class LIBIMEWENZHOUNESE_EXPORT WenzhouneseDictionary : public TrieDictionary {
public:
    static const size_t SystemDict = 0;
    static const size_t UserDict = 1;
    explicit WenzhouneseDictionary();
    ~WenzhouneseDictionary();

    // Load dicitonary for a specific dict.
    void load(size_t idx, std::istream &in, WenzhouneseDictFormat format);
    void load(size_t idx, const char *filename, WenzhouneseDictFormat format);

    // Match the word by encoded wenzhounese.
    void matchWords(const char *data, size_t size,
                    WenzhouneseMatchCallback callback) const;

    void save(size_t idx, const char *filename, WenzhouneseDictFormat format);
    void save(size_t idx, std::ostream &out, WenzhouneseDictFormat format);

    void addWord(size_t idx, std::string_view fullWenzhounese,
                 std::string_view hanzi, float cost = 0.0f);

    using dictionaryChanged = TrieDictionary::dictionaryChanged;

protected:
    void
    matchPrefixImpl(const SegmentGraph &graph,
                    const GraphMatchCallback &callback,
                    const std::unordered_set<const SegmentGraphNode *> &ignore,
                    void *helper) const override;

private:
    void loadText(size_t idx, std::istream &in);
    void loadBinary(size_t idx, std::istream &in);
    void saveText(size_t idx, std::ostream &out);

    std::unique_ptr<WenzhouneseDictionaryPrivate> d_ptr;
    FCITX_DECLARE_PRIVATE(WenzhouneseDictionary);
};

} // namespace wenzhounese
} // namespace libime

#endif // _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEDICTIONARY_H_
