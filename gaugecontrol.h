#ifndef GAUGECONTROL_H
#define GAUGECONTROL_H

#include <QObject>
#include <QtQml>
#include <QDebug>
#include <iostream>
#include <cmath>

class GaugeControl : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int speed READ speed NOTIFY speedChanged)
  Q_PROPERTY(QVariantList speedArray READ speedArray NOTIFY speedArrayChanged)
  Q_PROPERTY(qreal averageSpeed READ averageSpeed NOTIFY averageSpeedChanged)
  Q_PROPERTY(qreal distance READ distance NOTIFY distanceChanged)
  QML_NAMED_ELEMENT(GaugeControl)
public:
  explicit GaugeControl(QObject *parent = nullptr);
  explicit GaugeControl(std::istream *stream);

  ~GaugeControl();

  int speed() const;
  const QVariantList &speedArray() const;
  qreal averageSpeed() const;
  qreal distance() const;

signals:

  void speedChanged(int);
  void speedArrayChanged();
  void averageSpeedChanged();
  void distanceChanged();

public slots:

  void writeLogFile();

private:
  int m_speed;
  std::istream *m_inputStream;
  std::size_t m_time;
  QTimer *m_speedTimer;

  // Roughly copied from material
  void calculate_speed();
  auto to_radians(double degrees);

  void readStream();
  QVariantList m_speedArray;
  qreal m_averageSpeed;
  qreal m_distance;
};

#endif // SPEEDGAUGECONTROL_H
