# IMOL · rra

早期 Qt 版本的 IMOL 框架，保留原有数据树、命令、网络、状态、翻译、版本注册，以及部件和插件框架。

## 内容

- `include/imol/`、`src/`：IMOL 核心。
- `include/bwaf/`、`widgets/`：IMOL 的部件和插件框架。
- `examples/basic.cpp`：节点读写、信号、子树监听和 JSON 往返示例。

`snapshot/rra` 为提取后的源码快照。Git 历史仅保留框架相关文件；当前分支在此基础上补充独立构建入口，保留原有接口。

## 构建

需要 Qt 5.15（Core、Network）、C++17 和 CMake 3.16+。此分支保留 Qt 5 API。

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt5
cmake --build build --config Release
```

默认构建 `imol` 和 `imol_example`。使用 `-DIMOL_BUILD_WIDGETS=ON` 可构建部件库 `imol_widgets`，此时还需 Qt Widgets。静态库使用 `-DBUILD_SHARED_LIBS=OFF`。

作为依赖使用 `add_subdirectory(path/to/imol)`，链接 `imol::core`，按需链接 `imol::widgets`。

本次整理未在 Qt 5 环境中编译验证。需要较新 Qt 版本时使用 `main` 分支；无 Qt 的下位机实现位于 `rxc`。

许可见 [LICENSE](LICENSE)。
