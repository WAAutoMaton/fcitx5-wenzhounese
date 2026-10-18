/*
 * SPDX-FileCopyrightText: 2017~2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#ifndef _LIBIME_WENZHOUNESE_WENZHOUNESEDATA_H_
#define _LIBIME_WENZHOUNESE_WENZHOUNESEDATA_H_

#include "libimewenzhounese_export.h"
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index_container.hpp>
#include <libime/wenzhounese/wenzhouneseencoder.h>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace libime {
namespace wenzhounese {
struct WenzhouneseHash {
    std::size_t operator()(std::string_view const &val) const {
        return boost::hash_range(val.begin(), val.end());
    }
};

class WenzhouneseEntry {
public:
    WenzhouneseEntry(const char *wenzhounese, WenzhouneseInitial initial,
                     WenzhouneseFinal final, bool fuzzy = false)
        : wenzhounese_(wenzhounese), initial_(initial), final_(final),
          fuzzy_(fuzzy) {}

    std::string_view wenzhouneseView() const { return wenzhounese_; }
    const std::string &wenzhounese() const { return wenzhounese_; }
    WenzhouneseInitial initial() const { return initial_; }
    WenzhouneseFinal final() const { return final_; }
    bool fuzzy() const { return fuzzy_; }

private:
    std::string wenzhounese_;
    WenzhouneseInitial initial_;
    WenzhouneseFinal final_;
    bool fuzzy_;
};

using WenzhouneseMap = boost::multi_index_container<
    WenzhouneseEntry,
    boost::multi_index::indexed_by<boost::multi_index::hashed_non_unique<
        boost::multi_index::const_mem_fun<WenzhouneseEntry, std::string_view,
                                          &WenzhouneseEntry::wenzhouneseView>,
        WenzhouneseHash>>>;

LIBIMEWENZHOUNESE_EXPORT
const WenzhouneseMap &getWenzhouneseMap();
LIBIMEWENZHOUNESE_EXPORT const std::vector<bool> &getEncodedInitialFinal();

LIBIMEWENZHOUNESE_EXPORT const
    std::unordered_map<std::string, std::pair<std::string, std::string>> &
    getInnerSegment();

} // namespace wenzhounese
} // namespace libime

#endif // _LIBIME_WENZHOUNESE_WENZHOUNESEDATA_H_
