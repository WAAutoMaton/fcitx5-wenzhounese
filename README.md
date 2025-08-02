# Libime Wenzhounese

一个基于 libime 的温州话输入法。使用沈克成先生的拼音方案。

代码基于 [libime-jyutping](https://github.com/fcitx/libime-jyutping) 修改而来。

## Build / 构建

```bash
cmake -B build -S . \
    -GNinja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DCMAKE_INSTALL_LIBDIR=lib

cmake --build build
```

## Credit / 鸣谢

项目中使用的字典文件来自 libpinyin, libime-jyutping, 和 [rime-wenzhounese](https://github.com/ZWolken/rime-wenzhounese)，并使用 GPLv3+ 协议发布。