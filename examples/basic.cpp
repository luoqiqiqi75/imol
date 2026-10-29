#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <imol/modulemanager.h>
#include <iostream>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    auto* root = imol::m().regist(&app, "demo");
    root->set(&app, "answer", 0);
    auto* answer = imol::m("demo.answer");

    int changes = 0;
    QObject::connect(answer, &imol::ModuleObject::changed, &app,
        [&changes](const QVariant& value, const QVariant& previous, QObject*) {
            ++changes;
            std::cout << "answer: " << previous.toInt() << " -> " << value.toInt() << '\n';
        });

    int activations = 0;
    root->watch(true, true);
    QObject::connect(root, &imol::ModuleObject::activated, &app,
        [&activations](imol::ModuleObject*, int, QObject*) { ++activations; });
    answer->set(&app, 42);
    if (answer->getInt() != 42 || changes != 1 || activations != 1)
        return 1;

    const auto json = root->exportToJson();
    imol::ModuleObject copy("copy");
    copy.importFromJson(&app, json);
    if (copy.r("answer")->getInt() != 42)
        return 2;
    std::cout << QJsonDocument(json.toObject()).toJson(QJsonDocument::Compact).constData() << '\n';
    return 0;
}
