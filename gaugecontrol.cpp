#include "gaugecontrol.h"

GaugeControl::GaugeControl(QObject *parent)
  : QObject{parent}, m_time(0), m_inputStream(nullptr), m_inputDevice(nullptr), m_value(0), m_maxValue(0), m_distance(0.0), m_averageValue(0.0)
{
  // Connect statistics gathering to value change
  QObject::connect(this, &GaugeControl::valueChanged, this, &GaugeControl::updateStatistics);
  // Connect log-writing to when parent terminating
  QObject::connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(writeLogFile()));
  for (QString arg : qApp->arguments()) {
    if (arg == "-s" || arg == "--simulated") {
      startSimulation();
      return;
    }
  }
}

GaugeControl::~GaugeControl()
{
  qDebug() << "Destructor called.";
  // Close input if set
  if (m_inputDevice != nullptr)
      m_inputDevice->close();
}

// Write a json log of run statistics
void GaugeControl::writeLogFile() {
  QJsonObject log;
  QDateTime now(QDateTime::currentDateTime());
  log.insert("quitTime", now.toSecsSinceEpoch());
  log.insert("runTime", m_time * 0.05);
  log.insert("distance", m_distance);
  log.insert("averageValue", m_averageValue);
  QJsonArray jsonArray;
  for (qreal qr : m_valueArray) {
      jsonArray.push_back(qr);
    }
  log.insert("valueArray", jsonArray);
  QJsonDocument logDoc;
  logDoc.setObject(log);
  QFile file("log" + QString::number(now.toSecsSinceEpoch()));
  file.open(QIODevice::WriteOnly);
  file.write(logDoc.toJson());
  file.close();
}

auto GaugeControl::to_radians(double degrees) {
  return degrees/180.0 * M_PI;
}

void GaugeControl::updateStatistics()
{
  m_valueArray[m_value] += 0.05;
  m_averageValue += m_value; // Running average
  m_averageValue = m_averageValue / m_time;
  m_distance += m_value / 3.6 * 0.05;
  emit valueArrayChanged();
  emit averageValueChanged();
  emit distanceChanged();
}

// Simulationmodel given in material
void GaugeControl::calculate_value() {
  auto t = to_radians(m_time);
  // I thought "max_value" would be the maximum, but sometimes output larger that max
  // Actual limit is 3.25 / 3.0 = 1.083....
  m_value = static_cast<int>((abs(sin(t)+sin(4*t)/4+2*sin(t/16))/3 / (3.25 / 3.0)) * m_maxValue);
  emit valueChanged(m_value);
}

void GaugeControl::readStream() {
  // First check if there is anything to read
  if (m_inputStream->status() == QTextStream::Ok) {
      QString buffer = m_inputStream->readLine();
      if (buffer.length() > 0) {
          // Ignore values beyond limit
          int newValue = buffer.toInt();
          if (newValue < m_valueArray.size()) {
              m_value = buffer.toInt();
              emit valueChanged(m_value);
            }
        }
    }
}

int GaugeControl::value() const
{
  return m_value;
}

const QList<qreal> &GaugeControl::valueArray() const
{
  return m_valueArray;
}

qreal GaugeControl::averageValue() const
{
  return m_averageValue;
}

qreal GaugeControl::distance() const
{
  return m_distance;
}

void GaugeControl::setMaxValue(int newMaxValue)
{
  m_maxValue = newMaxValue;
  m_valueArray.resize(m_maxValue + 1); // make room for 0
}

// Simulate values from within
void GaugeControl::startSimulation()
{
  m_valueTimer = new QTimer(this);
  m_valueTimer->setInterval(50);
  QObject::connect(m_valueTimer, &QTimer::timeout,[this]() {
      m_time++;
      calculate_value();
    });
  qDebug() << "start timer from startSim";
  m_valueTimer->start();
}

void GaugeControl::setInputStream(QTextStream &inputStream)
{
  if (inputStream.status() == QTextStream::Ok) {
    m_inputStream = &inputStream;
  } else {
      return;
    }
  m_valueTimer = new QTimer(this);
  m_valueTimer->setInterval(50);
  QObject::connect(m_valueTimer, &QTimer::timeout,
    [this]() {
      m_time++;
      readStream();
    });
  m_valueTimer->start();
}

void GaugeControl::setInputDevice(QIODevice &device)
{
  if (device.isOpen() || device.open(QIODeviceBase::ReadOnly)) {
      m_inputDevice = &device;
    }
  else {
    return;
    }
  // Set inputstream based on device (tried to use readLine() from device, couldn't make it work quickly)
  QTextStream *stream = new QTextStream(&device);
  setInputStream(*stream);
}

void GaugeControl::setStandardInput()
{
  QTextStream *stream = new QTextStream(stdin);
  setInputStream(*stream);
}
