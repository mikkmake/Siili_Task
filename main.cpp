#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <csignal>
#include "gaugecontrol.h"

void sigIntHandler(int signal) {

}
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

// Check if we only want to simulate gauge-data
bool simulationSwitch(int &argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--simulated") == 0) {
      return true;
    }
  }
  return false;
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
    // Register GaugeControl as QML-type
    qmlRegisterType<GaugeControl>("GaugeControl", 1, 0, "GaugeControl");

    engine.load(url);
    return guiApp->exec();
  } else {
    qDebug() << "Non-graphical init";
    // Initiate GaugeControl
    GaugeControl speedControl(app.get());
    speedControl.maxValue(190);
    // Push received values to stdout
    QObject::connect(&speedControl, &GaugeControl::valueChanged,
      [](int value) {
        std::cout << value << std::endl;
      });
    // In terminal we exit by Ctrl-C, but still want to get quitting signal from QApplication
    std::signal(SIGINT, [](int sigInt) {
        qDebug() << "quitting by SIGINT";
        std::signal(sigInt, SIG_DFL); // why doe?
        qApp->quit();
      });
    return app->exec();
  }
}
