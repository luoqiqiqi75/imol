#include <QCoreApplication>
#include <QElapsedTimer>
#include <QThread>
#include <imol/xservice.h>
#include <core/common.h>
#include <iostream>

template<class F> bool waitFor(F condition)
{
    QElapsedTimer timer;
    timer.start();
    while (!condition() && timer.elapsed() < 4000) {
        QCoreApplication::processEvents();
        QThread::msleep(2);
    }
    return condition();
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    if (app.arguments().size() != 2) return 10;
    XService client;
    client.startSetup();
    client.startInit();
    client.mobj()->r("host")->set(&app, "ip", "127.0.0.1");
    client.mobj()->r("host")->set(&app, "port", app.arguments()[1].toInt());
    client.mobj()->r("trigger.connect")->trigger();
    if (!waitFor([&] { return client.mobj()->c("connected")->getBool(); })) return 11;
    if (client.get(&app, "example")->r("value")->getInt() != 12) return 12;

    ix::Node input("input");
    input.set(&app, "value", 99);
    auto* setResult = client.set(&app, "example", &input);
    if (!setResult->get().isValid() || setResult->getInt(-1) != 0) return 13;
    if (client.get(&app, "example")->r("value")->getInt() != 99) return 14;

    client.watch(&app, "example", true);
    if (!waitFor([&] { return client.dataNode("example")->r("value")->getInt() == 100; })) return 15;
    client.watch(&app, "example", false);
    if (client.execCommand(&app, "echo", QVariant(7))->r("data")->getInt() != 7) return 16;
    auto* missing = client.get(&app, "missing");
    if (missing->getInt() != -0x0102) return 17;
    client.startDetach();
    std::cout << "xservice handshake/get/set/watch/unwatch/command/error passed\n";
    return 0;
}
