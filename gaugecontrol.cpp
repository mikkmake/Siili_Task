#include "gaugecontrol.h"

GaugeControl::GaugeControl(QObject *parent)
  : QObject{parent}, m_time(0), m_speed(0), m_inputStream(nullptr)
{
  qDebug() << "Control constructing without stream";
  m_speedTimer = new QTimer(this);
  m_speedTimer->setInterval(50);
  // If no input stream given, generate speed ourselves
  QObject::connect(m_speedTimer, &QTimer::timeout,[this]() {
      m_time++;
      calculate_speed();
    });
  m_speedTimer->start();
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

void GaugeControl::calculate_speed() {
  auto t = to_radians(m_time);
  m_speed = static_cast<int>(abs(sin(t)+sin(4*t)/4+2*sin(t/16))/3 * 190.0);
  qDebug() << "emitting speedchanged";
  emit speedChanged(m_speed);
}

void GaugeControl::readStream() {
  // First check if there is anything
  std::string buffer;
  if (getline(*m_inputStream, buffer)) {
      m_speed = std::stoi(buffer);
      emit speedChanged(m_speed);
    }
}

int GaugeControl::speed() const
{
  return m_speed;
}

const QVariantList &GaugeControl::speedArray() const
{
  return m_speedArray;
}

qreal GaugeControl::averageSpeed() const
{
  return m_averageSpeed;
}

qreal GaugeControl::distance() const
{
  return m_distance;
}
