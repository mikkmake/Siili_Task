#include "gaugecontrol.h"

GaugeControl::GaugeControl(QObject *parent)
  : QObject{parent}, m_time(0), m_inputStream(NULL), m_value(0), m_maxValue(0)
{
  // Connect log-writing to when parent terminating
  QObject::connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(writeLogFile()));
  for (QString arg : qApp->arguments()) {
    if (arg == "-s" || arg == "--simulated") {
      startSimulation();
      return;
    }
  }
  // Default to stdin
  m_inputStream = new QTextStream(stdin);
  setInputStream(*m_inputStream);
}

GaugeControl::~GaugeControl()
{
  qDebug() << "Destructor called.";
}

void GaugeControl::writeLogFile() {
  qDebug() << "Log file writer called";
}

auto GaugeControl::to_radians(double degrees) {
  return degrees/180.0 * M_PI;
}

void GaugeControl::calculate_value() {
  auto t = to_radians(m_time);
  m_value = static_cast<int>(abs(sin(t)+sin(4*t)/4+2*sin(t/16))/3 * m_maxValue);
  emit valueChanged(m_value);
}

void GaugeControl::readStream() {
  // First check if there is anything to read
  // QTextStream qin(stdin);
  QString buffer = m_inputStream->readLine();
  if (m_inputStream->status() == QTextStream::Ok) {
      if (buffer.length() > 0) {
          m_value = buffer.toInt();
          emit valueChanged(m_value);
        }
    }
}

int GaugeControl::value() const
{
  return m_value;
}

const QVariantList &GaugeControl::valueArray() const
{
  return m_valueArray;
}

qreal GaugeControl::averageValue() const
{
  return m_averageSpeed;
}

qreal GaugeControl::distance() const
{
  return m_distance;
}

void GaugeControl::maxValue(int newMaxValue)
{
  m_maxValue = newMaxValue;
}

void GaugeControl::startSimulation()
{
  m_valueTimer = new QTimer(this);
  m_valueTimer->setInterval(50);
  // If no input stream given, generate value ourselves
  QObject::connect(m_valueTimer, &QTimer::timeout,[this]() {
      m_time++;
      calculate_value();
    });
  qDebug() << "start timer from startSim";
  m_valueTimer->start();
}

void GaugeControl::setInputStream(QTextStream &inputStream)
{
  m_inputStream = &inputStream;
  m_valueTimer = new QTimer(this);
  m_valueTimer->setInterval(50);
  QObject::connect(m_valueTimer, &QTimer::timeout,
    [this]() {
      readStream();
    });
  qDebug() << "start timer from setStream";
  m_valueTimer->start();
}
