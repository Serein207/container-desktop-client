#include "Infrastructure/Network/NetworkClient.h"
#include <QFontDatabase>
#include <QGuiApplication>
#include <QIcon>
#include <QNetworkCookieJar>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[]) {
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");
    QGuiApplication::setOrganizationName("GaoDiZhengGeGuo");
    QGuiApplication::setOrganizationDomain("https://container-desk.top");
    QGuiApplication::setApplicationName("Container Desktop");

    QGuiApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(QStringLiteral(":/res/icon/app.png")));

    auto fontId =
        QFontDatabase::addApplicationFont(":/res/fonts/MiSans-Regular.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    if (!fontFamilies.empty())
        QGuiApplication::setFont(QFont(fontFamilies[0]));

    ContainerDesktop::NetworkClient::getInstance()->setCookieJar(
        new QNetworkCookieJar);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/ui/window/MainWindow.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
