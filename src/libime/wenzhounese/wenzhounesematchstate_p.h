/*
 * SPDX-FileCopyrightText: 2018~2018 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEMATCHSTATE_P_H_
#define _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEMATCHSTATE_P_H_

#include <fcitx-utils/macros.h>
#include <libime/core/lattice.h>
#include <libime/core/lrucache.h>
#include <libime/wenzhounese/wenzhounesedictionary.h>
#include <libime/wenzhounese/wenzhounesematchstate.h>
#include <memory>
#include <unordered_map>

namespace libime {

namespace wenzhounese {

using WenzhouneseTriePosition = std::pair<uint64_t, size_t>;
using WenzhouneseTriePositions = std::vector<WenzhouneseTriePosition>;

// Matching result for a specific WenzhouneseTrie.
struct MatchedWenzhouneseTrieNodes {
    MatchedWenzhouneseTrieNodes(const WenzhouneseTrie *trie, size_t size)
        : trie_(trie), size_(size) {}
    FCITX_INLINE_DEFINE_DEFAULT_DTOR_COPY_AND_MOVE(MatchedWenzhouneseTrieNodes)

    const WenzhouneseTrie *trie_;
    WenzhouneseTriePositions triePositions_;

    // Size of syllables.
    size_t size_;
};

// A cache to store the matched word, encoded Full Wenzhounese for this word and
// the
// adjustment score.
struct WenzhouneseMatchResult {
    WenzhouneseMatchResult(std::string_view s, float value,
                           std::string_view encodedWenzhounese)
        : word_(s, InvalidWordIndex), value_(value),
          encodedWenzhounese_(encodedWenzhounese) {}
    WordNode word_;
    float value_;
    std::string encodedWenzhounese_;
};

// class to store current SegmentGraphPath leads to this match and the match
// reuslt.
struct MatchedWenzhounesePath {
    MatchedWenzhounesePath(const WenzhouneseTrie *trie, size_t size,
                           SegmentGraphPath path)
        : result_(std::make_shared<MatchedWenzhouneseTrieNodes>(trie, size)),
          path_(std::move(path)) {}

    MatchedWenzhounesePath(std::shared_ptr<MatchedWenzhouneseTrieNodes> result,
                           SegmentGraphPath path)
        : result_(result), path_(std::move(path)) {}

    FCITX_INLINE_DEFINE_DEFAULT_DTOR_COPY_AND_MOVE(MatchedWenzhounesePath)

    auto &triePositions() { return result_->triePositions_; }
    const auto &triePositions() const { return result_->triePositions_; }
    const WenzhouneseTrie *trie() const { return result_->trie_; }

    // Size of syllables. not necessarily equal to size of path_, because there
    // may be separators.
    auto size() const { return result_->size_; }

    std::shared_ptr<MatchedWenzhouneseTrieNodes> result_;
    SegmentGraphPath path_;
};

// This need to be keep sync with WenzhouneseSegmentGraphPathHasher
class WenzhouneseStringHasher {
public:
    size_t operator()(const std::string &s) const {
        boost::hash<char> hasher;

        size_t seed = 0;
        for (char c : s) {
            boost::hash_combine(seed, hasher(c));
        }
        return seed;
    }
};

// A list of all search paths
typedef std::vector<MatchedWenzhounesePath> MatchedWenzhounesePaths;

// Map from SegmentGraphNode to Search Paths.
typedef std::unordered_map<const SegmentGraphNode *, MatchedWenzhounesePaths>
    NodeToMatchedWenzhounesePathsMap;

// A cache for all WenzhouneseTries. From a wenzhounese string to its matched
// WenzhouneseTrieNode
typedef std::unordered_map<
    const WenzhouneseTrie *,
    LRUCache<std::string, std::shared_ptr<MatchedWenzhouneseTrieNodes>,
             WenzhouneseStringHasher>>
    WenzhouneseTrieNodeCache;

// A cache for WenzhouneseMatchResult.
typedef std::unordered_map<
    const WenzhouneseTrie *,
    LRUCache<std::string, std::vector<WenzhouneseMatchResult>,
             WenzhouneseStringHasher>>
    WenzhouneseMatchResultCache;

class WenzhouneseMatchStatePrivate {
public:
    WenzhouneseMatchStatePrivate(WenzhouneseContext *context)
        : context_(context) {}

    WenzhouneseContext *context_;
    NodeToMatchedWenzhounesePathsMap matchedPaths_;
    WenzhouneseTrieNodeCache nodeCacheMap_;
    WenzhouneseMatchResultCache matchCacheMap_;
};

} // namespace wenzhounese
} // namespace libime

#endif // _LIBIME_WENZHOUNESE_LIBIME_WENZHOUNESE_WENZHOUNESEMATCHSTATE_P_H_
