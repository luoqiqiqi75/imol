# IMOL · rxc

IMOL 的无 Qt 下位机实现。此分支保留框架源码及原有接口，适合作为历史版本备份和后续移植参考。

## 内容

- `imol/base.h`：对象、信号、管理器、创建器和通用容器。
- `imol/datamanager.h`：类型化数据、JsonRef 和全局数据注册。
- `imol/commandmanager.h`：命令和 Procedure 执行链。
- `imol/module.h`、`imol/common.h`：模块生命周期和入口。
- `imol/loopmanager.h`、`imol/src/loop/`：实时、网络和线程池循环。
- `imol/netmanager.h`、`imol/src/net/`：网络连接和消息处理。

`snapshot/rxc` 标记原样提取的框架源码。25 个源码文件的 SHA-256 记录在 [source-manifest.json](source-manifest.json)，不包含业务适配层。

## 集成条件

此版本使用 C++11，没有独立构建入口。编译需要 JsonCpp、Boost、KDL、EVPP、Orocos RTT，以及日志、文件系统、UUID、线程池、并发队列和实时线程配置的配套接口。相关 include 保持原有写法，集成时需由目标环境提供这些依赖。

本次仅核验源码完整性，未验证编译或实时运行。需要独立构建的 Qt 版本时使用 `main`；早期 Qt 版本位于 `rra`。

原有许可声明见 [LICENSE](LICENSE) 及各源码文件。
