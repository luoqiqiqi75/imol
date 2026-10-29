# IMOL · rxc

IMOL 的无 Qt 下位机实现。此分支保留完整 IMOL 目录及 xservice，服务端 SDK 与 rra 分支 imolx 的 XService 客户端使用同一套协议。

## 内容

- `imol/base.h`：对象、信号、管理器、创建器和通用容器。
- `imol/datamanager.h`：类型化数据、JsonRef 和全局数据注册。
- `imol/commandmanager.h`：命令和 Procedure 执行链。
- `imol/module.h`、`imol/common.h`：模块生命周期和入口。
- `imol/loopmanager.h`、`imol/src/loop/`：实时、网络和线程池循环。
- `imol/netmanager.h`、`imol/src/net/`：网络连接和消息处理。
- `imol/xmodule.h`、`imol/xrobot.h`、`imol/xsupport.h`：原有模块整合、类型与平台支持接口。
- `xservice/sdk/`：TCP JSON 服务端、命令输入预处理 CIP 和协议错误码。
- `xservice/sys_socket/`：系统文本指令与查询通道。

38 个源码文件的 SHA-256 记录在 [source-manifest.json](source-manifest.json)。IMOL 与 xservice 文件均按原实现保留；xservice 的 7 个文件已逐一与来源文件校验一致。

## xservice 对接

SDK 默认监听 TCP 6666，以单个回车字节 `0x0D` 分帧，每帧为 JSON 对象。请求中的 `id` 在响应和异步通知中保留。

| 字段 | 用途 |
| --- | --- |
| `imol` | 枚举版本、数据键与命令键；客户端握手 ID 为 `@handshake` |
| `g` | 读数据，支持选择字段 |
| `s` | 合并写入数据 |
| `w` | 开始或取消订阅 |
| `n` | 服务端数据变更通知 |
| `c` | 执行命令，经过 CIP 后返回执行结果 |

握手请求示例（JSON 后追加回车）：

```json
{"id":"@handshake","imol":{"version":null,"data":null,"command":null}}
```

读数据与订阅示例：

```json
{"id":"example","g":{"example":null}}
{"id":"#example","w":{"example":true}}
```

数据变更通知对应 `{"id":"#example","n":{"example":...}}`。无附加数据的 Result 是整数（成功为 `0`）；有附加数据时是包含 `data`、按需包含 `error` 的对象，不能改成另一种统一响应包装。

SDK 的实现位于 `xservice/sdk/src/sdkmodule.cpp`，对外接口和 CIP 注册位于 `xservice/sdk/sdk_interface.h`。`sys_socket` 是独立的反向 TCP 文本通道，不是 imolx 客户端使用的 JSON SDK 入口。

## 集成条件

此版本使用 C++11，没有独立构建入口。编译时把当前目录加入 include 路径，保留 `imol/...`、`xservice/...` 的相对组织。需要 JsonCpp、Boost、KDL、EVPP、Orocos RTT，以及日志、文件系统、UUID、线程池、并发队列和实时线程配置的配套接口。

模块整合及系统文本通道还引用原有状态、运动、配置、驱动等平台接口。相关 include 和实现均保持原样，集成时由目标环境提供；这些平台实现不在本仓库内复制。

本次核验全部源码完整性，并在 rra 中使用真实 Qt 客户端对 SDK 报文夹具通过握手、读写、订阅、命令、错误和分包检查。尚未编译下位机整机或完成真实服务端环境的联调。

`snapshot/rxc` 为初次提取快照，`snapshot/rxc-xservice` 包含完整框架与服务端。需要独立 Qt 库时使用 `main`；完整 Qt 程序与 imolx 客户端位于 `rra`。

原有许可声明见 [LICENSE](LICENSE) 及各源码文件。
