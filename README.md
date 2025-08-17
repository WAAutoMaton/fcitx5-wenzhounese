# fcitx5-wenzhounese

一个基于 fcitx5 框架的温州话输入法。使用沈克成先生的拼音方案。

代码基于 [libime-jyutping](https://github.com/fcitx/libime-jyutping) 修改而来。

## 构建

```bash
cmake -B build -S . \
    -GNinja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DCMAKE_INSTALL_LIBDIR=lib

cmake --build build
```

## 拼音方案

### 声母表

| 音标 | 拼音 |
| --- | --- |
| [p] | b |
| [p'] | p |
| [b] | bb |
| [m] | m |
| [f] | f |
| [v] | v |
| [t] | d |
| [t'] | t |
| [d] | dd |
| [n] | n |
| [l] | l |
| [ts] | z |
| [ts'] | c |
| [dz] | zz |
| [s] | s |
| [z] | ss |
| [ʨ] | j |
| [ʨ'] | q |
| [ʥ] | jj |
| [ȵ] | ny |
| [ɕ] |x |
| [ʑ] | xx |
| [k] | g |
| [k'] | k |
| [g] | gg |
| [ŋ] | ng |
| [h] | h |
| [ɦ] | hh |
| [u] | u |
| [v] | v |
| [i] | i |
| [j] | y |

### 韵母表

TODO...

| 音标 | 拼音 |
| --- | --- |
| [a] | a |
| [] | ae |
| [] | ai |
|  | ang |
|  | au |
|  | e |
|  | ee |
|  | ei |
|  | eng |
|  | eu |
|  | i |
|  | ia |
|  | iae |
|  | iai |
|  | iang |
|  | iau |
|  | ieu |
|  | ii |
|  | iong | 
|  | o |
|  | oe |
|  | oev |
|  | ong |
|   | u |
|  | uio |
|  | v |
|  | uio |
|  | yo |

## 鸣谢

项目中使用的字典文件来自 libpinyin, libime-jyutping, 和 [rime-wenzhounese](https://github.com/ZWolken/rime-wenzhounese)，并使用 GPLv3+ 协议发布。
