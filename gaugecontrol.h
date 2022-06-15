#ifndef GAUGECONTROL_H
#define GAUGECONTROL_H

#include <QObject>
#include <QtQml>
#include <QDebug>
#include <iostream>
#include <cmath>
#include <string>

class GaugeControl : public QObject
{
  Q_OBJECT
  // Named value, not speed, because this is generic gauge control
  Q_PROPERTY(int value READ value NOTIFY valueChanged)
  Q_PROPERTY(QList<int> valueArray READ valueArray NOTIFY valueArrayChanged)
  Q_PROPERTY(qreal averageValue READ averageValue NOTIFY averageValueChanged)
  Q_PROPERTY(qreal distance READ distance NOTIFY distanceChanged)
  Q_PROPERTY(int maxValue WRITE maxValue)
  QML_NAMED_ELEMENT(GaugeControl)
public:
  explicit GaugeControl(QObject *parent = nullptr);

  ~GaugeControl();

  int value() const;
  const QList<int> &valueArray() const;
  qreal averageValue() const;
  qreal distance() const;

  void maxValue(int newMaxValue);
  void startSimulation();

  void setInputStream(QTextStream &inputStream);

signals:

  void valueChanged(int);
  void valueArrayChanged();
  void averageValueChanged();
  void distanceChanged();

public slots:

  void writeLogFile();

private:
  int m_value;
  QTextStream *m_inputStream;
  std::size_t m_time;
  QTimer *m_valueTimer;

  // Roughly copied from material
  void calculate_value();
  auto to_radians(double degrees);

  void readStream();
  QList<int> m_valueArray;
  qreal m_averageValue;
  qreal m_distance;
  int m_maxValue;
};

#endif // SPEEDGAUGECONTROL_H
