#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "gaugecontrol.h"

// Determine whether to run graphically or not
// Essentially from https://doc.qt.io/qt-6/qapplication.html
QCoreApplication* createApplication(int &argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--graphical") == 0) {
      return new QGuiApplication(argc, argv);
    }
  } // Not graphics-option -> run without by default
  return new QCoreApplication(argc, argv);
}

int main(int argc, char *argv[])
{
  QScopedPointer<QCoreApplication> app(createApplication(argc, argv));

  if (qobject_cast<QGuiApplication*>(app.get())) {
    // Initiate graphical application
    qDebug() << "Graphical init";
    QGuiApplication *guiApp = qobject_cast<QGuiApplication*>(app.get());

    // IDE-generated init and check
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/Siili_Task/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     guiApp, [url](QObject *obj, const QUrl &objUrl) {
      if (!obj && url == objUrl)
        QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    // Register custom GaugeControl as QML-type
    qmlRegisterType<GaugeControl>("GaugeControl", 1, 0, "GaugeControl");

    engine.load(url);
    return guiApp->exec();
  } else {
    qDebug() << "Non-graphical init";
    // Initiate GaugeControl
    GaugeControl speedControl;
    QObject::connect(&speedControl, &GaugeControl::speedChanged,
      [](int speed) {
        std::cout << speed << std::endl;
      });
    return app->exec();
  }
}
