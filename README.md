# IMOL · rra

这一分支保留完整的 Qt 5 程序组织：框架核心、部件框架、主程序和基础扩展一起构建。默认启动 IMOL 主窗口并加载 imolx，不包含业务模块。

```text
imol.pro                    qmake 总工程
imol/
  imol.pro                  core → bwaf → program
  include/core/             数据树、信号、命令、网络、状态、翻译
  include/bwaf/             插件与部件接口
  src/core/
  src/bwaf/
  src/program/              主窗口、启动、终端、配置与资源
imolx/
  imolx.pro                 基础扩展插件
  src/                      主题、导航、节点、xservice 客户端和支持接口
  template/                 基础数据模板
runtime/                    只加载 imolx 的启动配置
```

## 构建和启动

需要 Qt 5.15、C++17 和 CMake 3.16+。Windows 使用 MSVC，包含主程序原有的崩溃处理支持。

```sh
cmake -S . -B build-rra -DCMAKE_PREFIX_PATH=/path/to/Qt5 -DCMAKE_BUILD_TYPE=Release
cmake --build build-rra --config Release
cd build-rra/bin
./imolProgram
```

构建生成 imolCore、imolBwaf、imolx 和 imolProgram。程序、插件和运行配置位于同一目录；运行前将 Qt 的 bin 加入 PATH。

也可以用 Qt Creator 打开根目录的 imol.pro。qmake 构建后，将 runtime/config.ini 和 imol/src/program/exec/internal_license.json 放到输出目录。此 JSON 默认使用 Windows 插件名 imolx.dll；其他平台需改为实际插件文件名。

默认配置提供顶部菜单、工具栏、导航栏、侧栏、中心区域和状态栏。xsupport.type=none 让基础界面离线启动，保留 xservice 客户端，可手动连接 rxc 的 SDK 服务（TCP 6666）。Qt 部件区的内容由后续插件填充。

原有启动配置生成和校验流程保留。首次启动会根据 internal_license.json 生成本机的 imolProgram.lic。

imolProgram --smoke-test 检查主程序、imolx 及其基础组件进入 IDLE，生成 smoke-test.json 和主窗口截图后退出。

## xservice

imolx/include/imol/xservice.h 是客户端入口，支持 get、set、watch、execCommand。传输为 TCP 上以回车字节分帧的 JSON，握手 ID 为 @handshake，默认对接端口为 6666。服务端和 CIP 接口保留在 rxc 分支的 xservice/sdk/。

snapshot/rra 为早期提取快照；本分支后续提交恢复完整工程与基础运行配置。许可见 LICENSE。

## 验证

已在 Qt 5.15.2 / MSVC 19.50 / Windows x64 下分别通过 CMake、qmake/nmake 构建和启动检查，确认基础组件进入 IDLE、主窗口及导航栏显示、业务模块数为 0。终端保留原有 Ctrl+Shift+T 快捷键。

启用 `-DIMOL_BUILD_TESTS=ON` 后，可以运行客户端协议检查：

```sh
python tests/xservice_wire.py build-rra/bin/imol_xservice_client.exe
```

测试运行真实 Qt 客户端，使用与 SDK 源码一致的服务端报文夹具，覆盖握手、读写、订阅、取消订阅、命令、错误、回车分帧与分包。此检查不代表已运行下位机整机环境。
