#include "gaugecontrol.h"

GaugeControl::GaugeControl(QObject *parent)
  : QObject{parent}, m_time(0), m_value(0), m_inputStream(nullptr), m_maxValue(0)
{
  qDebug() << "Control constructing without stream";
  qDebug() << qApp->arguments();
  for (QString arg : qApp->arguments()) {
    if (arg == "-s" || arg == "--simulated") {
      startSimulation();
      return;
    }
  }
  setInputStream(std::cin);
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
  std::string buffer;
  if (getline(*m_inputStream, buffer)) {
      m_value = std::stoi(buffer);
      emit valueChanged(m_value);
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
  m_valueTimer->start();
}

void GaugeControl::setInputStream(std::istream &inputStream)
{
  m_inputStream = &inputStream;
  m_valueTimer = new QTimer(this);
  m_valueTimer->setInterval(50);
  QObject::connect(m_valueTimer, &QTimer::timeout,
    [this]() {
      readStream();
    });
  m_valueTimer->start();
}
