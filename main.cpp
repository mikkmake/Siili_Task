#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <csignal>
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

static void catchSigInt(int sigInt) {
  qInfo() << "readLine() blocks quit(). Press enter to quit.";
  QCoreApplication::quit();
}

int main(int argc, char *argv[])
{
  QScopedPointer<QCoreApplication> app(createApplication(argc, argv));

  if (qobject_cast<QGuiApplication*>(app.get())) {
    // Initiate graphical application
    QGuiApplication *guiApp = qobject_cast<QGuiApplication*>(app.get());

    // IDE-generated init and check
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/Siili_Task/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     guiApp, [url](QObject *obj, const QUrl &objUrl) {
      if (!obj && url == objUrl)
        QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    // Register GaugeControl as QML-type
    qmlRegisterType<GaugeControl>("GaugeControl", 1, 0, "GaugeControl");

    engine.load(url);
    return guiApp->exec();
  } else {
    // Initiate GaugeControl
    GaugeControl speedControl(app.get());
    speedControl.setMaxValue(190);
    speedControl.setStandardInput();
    // Push received values to stdout
    QObject::connect(&speedControl, &GaugeControl::valueChanged,
      [](int value) {
        std::cout << value << std::endl;
      });
    // In terminal we exit by Ctrl-C, but still want to get quitting signal from QApplication
    // std::signal(SIGINT, [](int sigInt) {
        // qDebug() << "caight sigint";
        // std::signal(sigInt, SIG_DFL); // why doe?
        // qApp->quit();
      // });
    std::signal(SIGINT, catchSigInt);
    return app->exec();
  }
}
