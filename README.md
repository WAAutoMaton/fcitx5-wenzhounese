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

### Linux Nightly 插件

[Nightly Build](https://github.com/WAAutoMaton/fcitx5-wenzhounese/releases/tag/latest) 同时提供 `Fcitx5-Wenzhounese-<构建号>-archlinux-x86_64.tar.zst` 和 SHA256 校验文件。该包使用 Arch Linux x86_64 和 GCC 构建，包含温州话插件、私有共享库、词典、语言模型及翻译，使用标准 `/usr` 安装布局。

这不是跨发行版通用包，也不是可直接交给 `pacman -U` 的 Arch 软件包。使用前需安装 ABI 兼容的 Fcitx5、libime、fcitx5-chinese-addons、Boost 和 zstd；构建依赖的源码提交号及系统库版本记录在包内的 `usr/share/doc/fcitx5-wenzhounese/build-info.json`。其他发行版或较旧系统建议从源码构建。

先验证 SHA256，再解压到临时目录检查文件；确认依赖兼容后，将其中的 `usr` 目录内容安装到系统 `/usr`，然后重启 Fcitx5，在配置工具中添加温州话。手动安装不会纳入包管理器，更新或卸载时需按归档中的文件列表管理。

### Windows 插件

使用 `fcitx5-windows` 提供的 Core/libime SDK 独立构建，脚本位于本仓库的 `scripts/build-windows.ps1`，无需将输入法加入 Core 的构建目标。

#### Nightly 下载

Windows x64 插件可从 [Nightly Build](https://github.com/WAAutoMaton/fcitx5-wenzhounese/releases/tag/latest) 下载，附带 SHA256 校验文件。将 ZIP 中的 `wenzhounese` 目录解压到 Core 安装目录的 `plugins`，或 `%LOCALAPPDATA%/Fcitx5/plugins`，然后重启服务并在 Settings 中添加温州话。

CI 使用 `fcitx5-windows` 的 `master` 构建匹配的 SDK，并验证打包后的插件能够加载和提交候选。包内的 `manifest.json` 记录插件和 Core SDK 的提交号，宿主需使用兼容版本。推送到 `master` 或手动运行 CI 会更新 `Nightly Build`；提交信息包含 `!release` 时跳过发布，PR 仅构建并上传 Actions artifact。

#### 前置条件

- CMake 3.27+、MSYS2 clang64，以及与 Core 相同的 ECM、gettext、Boost iostreams 和 zstd 等构建依赖。
- 启用插件加载的 `fcitx5-windows` 源码及其工具链子模块，默认位于同级目录 `../fcitx5-windows`。
- 与宿主匹配的 AMD64 Release Core/libime SDK。Windows 主仓库的统一构建脚本默认将其安装到 `build/all/prefix`，其中包含头文件、导入库、CMake package、模块公共接口和数据生成工具；仅有运行时 DLL 的部署目录不能代替 SDK。SDK 应保留在原安装位置使用。

#### 构建

在本仓库根目录的 Windows PowerShell 中运行：

```powershell
./scripts/build-windows.ps1 -CorePrefix ../fcitx5-windows/build/all/prefix
```

默认构建目录为本仓库的 `build/windows`，插件包生成在 `build/windows/package`。脚本只构建温州话引擎、私有库及其词库/模型，不重建 Core、libime 或 chinese-addons。

| 参数 | 用途 |
| --- | --- |
| `-CorePrefix` | 必填，匹配的 Core/libime SDK 安装前缀 |
| `-WindowsSourceDirectory` | Windows 主仓库源码路径，默认 `../fcitx5-windows`，用于读取通用 addon 工具链 |
| `-MSYS2Root` | MSYS2 根目录，默认 `C:/msys64` |
| `-DependencyPrefix` | 与 Core 构建相同的依赖前缀，默认 Windows 主仓库的 `build/deps/clang64` |
| `-BuildRoot` / `-Prefix` | 自定义构建目录和插件包输出目录 |
| `-SourceDirectory` | 构建其他源码副本，默认本仓库根目录 |
| `-Jobs` | 并行构建任务数，默认 6 |
| `-WhatIf` | 预览构建目标，不创建文件或运行构建 |

如果 Windows 主仓库位于其他位置，可显式指定：

```powershell
./scripts/build-windows.ps1 `
    -WindowsSourceDirectory D:/src/fcitx5-windows `
    -CorePrefix D:/src/fcitx5-windows/build/all/prefix `
    -DependencyPrefix D:/src/fcitx5-windows/build/deps/clang64
```

插件必须匹配 Core 的架构、Fcitx5/libime 版本及 Windows 补丁、Clang Windows GNU 工具链、libc++ 和 CRT，不能混用 MSVC 或 Linux/macOS 的二进制。插件复用宿主的公共运行库，不应覆盖 `libFcitx5*.dll`、`libIMECore.dll` 或 `libc++.dll`。

#### 安装与启用

1. 在输入法指示器菜单中选择 `关闭服务`。
2. 将 `build/windows/package` 中的 `bin`、`lib`、`share` 一起放入 Core 安装目录下的 `plugins/wenzhounese`，或 `%LOCALAPPDATA%/Fcitx5/plugins/wenzhounese`，保留目录结构。每个插件发布包使用独立目录，需要支持独立目录发现的新版 Core。
3. 选择 `重启服务`，在 Settings 的输入法列表中添加温州话，然后使用 `Ctrl+Space` 或指示器切换。

主要运行时文件如下；标点依赖由宿主的 chinese-addons 提供：

```text
plugins/wenzhounese/
  bin/libIMEWenzhounese.dll
  lib/fcitx5/libwenzhounese.dll
  lib/libime/wenzhounese.lm
  lib/libime/wenzhounese.lm.predict
  share/fcitx5/addon/wenzhounese.conf
  share/fcitx5/inputmethod/wenzhounese.conf
  share/libime/wenzhounese.dict
  share/locale/<语言>/LC_MESSAGES/fcitx5-wenzhounese.mo
```

更新或移除插件前同样先关闭服务，完成后重启。独立布局可替换或删除整个 `plugins/wenzhounese` 目录，用户配置和词库仍保存在原有用户数据目录。旧的共用布局仍兼容，但迁移时需要手动移除旧位置中属于温州话插件的文件，避免保留两份副本；不要删除其他插件的文件。详细的宿主搜索规则及通用验证工具见 Windows 主仓库的 `docs/windows-plugins.md`。

语言模型现在使用独立的 `wenzhounese.lm` / `wenzhounese.lm.predict` 文件名；升级时请同时安装新引擎和模型。

#### 隔离验证

按照 Windows 主仓库 `docs/windows-plugins.md` 构建通用 `addon_probe` 并安装到隔离 Core 树的 `bin` 后，可从本仓库根目录验证生成的插件包：

```powershell
$sdk = (Resolve-Path ../fcitx5-windows/build/all/prefix).Path.Replace('\', '/')
$plugin = (Resolve-Path ./build/windows/package).Path.Replace('\', '/')
& "$sdk/bin/addon_probe.exe" "$sdk" "$plugin" wenzhounese nyi
```

验证自动发现时，将包安装到隔离 Core 树的 `plugins/wenzhounese`，再运行：

```powershell
& "$sdk/bin/addon_probe.exe" "$sdk" --default wenzhounese nyi
```

该检查覆盖输入法发现、DLL 加载、配置读取和候选提交，不启动 Pipe 服务或注册 TSF；真实 Windows 应用中的 TSF 输入仍需单独验收。

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

| 音标 | 沈氏拼音 |
| --- | --- |
| [a] | a |
| [ɛ] | ae |
| [ai] | ai |
| [aŋ] | ang |
| [au] | au |
| [ə] | e |
| [e] | ee |
| [ei] | ei |
| [əŋ] | eng |
| [əu] | eu |
| [i] | i |
| [ia] | ia |
| [iɛ] | iae |
| [iai] | iai |
| [iaŋ] | iang |
| [iau] | iau |
| [iəu] | ieu |
| [ɿ] | ii |
| [ioŋ] | iong |
| [o] | o |
| [ø] | oe |
| [øy] | oev |
| [oŋ] | ong |
| [u] | u |
| [uɔ] | uio |
| [y] | ü |
| [yɔ] | üio |
| [yo] | üo |

## 鸣谢

项目中使用的字典文件来自 libpinyin, libime-jyutping, 和 [rime-wenzhounese](https://github.com/ZWolken/rime-wenzhounese)，并使用 GPLv3+ 协议发布。

语言模型沿用上游数据，源码文件为 `data/wenzhounese.arpa`。
