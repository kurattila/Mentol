#include "src/MentolLib/MainViewModel.h"
#include <QApplication>
#include <QQmlContext>
#include <QtQml> // qmlRegisterType<>()
#include <QQmlApplicationEngine>
#include "UnitTests/AutoTest.h"

int main(int argc, char *argv[])
{
    if (argc >= 2 && !qstricmp(argv[1], "/UnitTests"))
    {
        int testsArgc = 1;
        char *testsArgv[] = { argv[0] };
        return AutoTest::run(testsArgc, testsArgv);
    }
    else
    {
        QApplication app(argc, argv);

        qmlRegisterType<ComplexityToPrecision_Model>("com.quatso.mentolcomponents", 1, 0, "ComplexityToPrecision_Model");

        MainViewModel mainViewModel;
        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty("mainViewModel", QVariant::fromValue(&mainViewModel));
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

        return app.exec();
    }
}
