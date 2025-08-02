/*
 * SPDX-FileCopyrightText: 2017~2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "libime/wenzhounese/wenzhounesedictionary.h"
#include "libime/wenzhounese/wenzhouneseencoder.h"
#include <fstream>
#include <getopt.h>
#include <iostream>

void usage(const char *argv0) {
    std::cout << "Usage: " << argv0 << " [-d] <source> <dest>" << std::endl
              << "-d: Dump binary to text" << std::endl
              << "-h: Show this help" << std::endl;
}

int main(int argc, char *argv[]) {

    bool dump = false;
    int c;
    while ((c = getopt(argc, argv, "dh")) != -1) {
        switch (c) {
        case 'd':
            dump = true;
            break;
        case 'h':
            usage(argv[0]);
            return 0;
        default:
            usage(argv[0]);
            return 1;
        }
    }

    if (optind + 2 != argc) {
        usage(argv[0]);
        return 1;
    }
    using namespace libime;
    using namespace libime::wenzhounese;
    WenzhouneseDictionary dict;

    dict.load(WenzhouneseDictionary::SystemDict, argv[optind],
              dump ? WenzhouneseDictFormat::Binary : WenzhouneseDictFormat::Text);

    std::ofstream fout;
    std::ostream *out;
    if (strcmp(argv[optind + 1], "-") == 0) {
        out = &std::cout;
    } else {
        fout.open(argv[optind + 1], std::ios::out | std::ios::binary);
        out = &fout;
    }
    dict.save(WenzhouneseDictionary::SystemDict, *out,
              dump ? WenzhouneseDictFormat::Text : WenzhouneseDictFormat::Binary);
    return 0;
}
