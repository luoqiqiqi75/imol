# IMOL

IMOL（Infinite Modules On Line）是基于 Qt 的轻量模块框架。主分支保留 QObject / QVariant 数据树、路径访问、信号通知、序列化，以及命令、网络、状态和翻译管理。

## 分支

| 分支 | 内容 |
| --- | --- |
| `main` | 最后的完整 Qt 实现，适合简洁集成 |
| `rra` | 早期 Qt 实现，含可选的部件和插件框架 |
| `rxc` | 无 Qt 的下位机实现，保留对象、数据、命令、模块和循环框架源码 |

三个分支独立维护，各自保留相应版本的接口。`snapshot/qt`、`snapshot/rra`、`snapshot/rxc` 标记提取后的源码快照。历史只包含 IMOL 相关文件。

## 构建

需要 C++17、CMake 3.16+ 和 Qt 5.15 / Qt 6（Core、Network）。

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt
cmake --build build --config Release
```

默认构建动态库 `imol` 和示例 `imol_example`。关闭示例使用 `-DIMOL_BUILD_EXAMPLES=OFF`，构建静态库使用 `-DBUILD_SHARED_LIBS=OFF`。运行示例前将对应 Qt 的 `bin` 目录加入 `PATH`。

在已有 CMake 项目中使用：

```cmake
add_subdirectory(path/to/imol)
target_link_libraries(my_app PRIVATE imol::core)
```

## 使用

```cpp
#include <imol/modulemanager.h>

auto* root = imol::m().regist(context, "demo");
root->set(context, "answer", 42);
int answer = imol::m("demo.answer")->getInt();

QObject::connect(imol::m("demo.answer"), &imol::ModuleObject::changed,
    context, [](const QVariant& value, const QVariant&, QObject*) {
        qDebug() << value;
    });
```

`context` 为调用方的 `QObject*`。路径查询 `imol::m(path)` 不会自动创建节点；通过 `regist` 或 `set` 创建。完整的信号通知和 JSON 往返示例见 [examples/basic.cpp](examples/basic.cpp)。

主分支采用 LGPL-3.0，详见 [LICENSE](LICENSE)。历史分支保留各自源码的许可声明。
