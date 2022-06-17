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
  Q_PROPERTY(int value READ value NOTIFY valueChanged)
  Q_PROPERTY(QList<qreal> valueArray READ valueArray NOTIFY valueArrayChanged)
  Q_PROPERTY(qreal averageValue READ averageValue NOTIFY averageValueChanged)
  Q_PROPERTY(qreal distance READ distance NOTIFY distanceChanged)
  Q_PROPERTY(int maxValue WRITE setMaxValue)
  QML_NAMED_ELEMENT(GaugeControl)

public:
  explicit GaugeControl(QObject *parent = nullptr);

  ~GaugeControl();

  // GETS
  int value() const;
  const QList<qreal> &valueArray() const;
  qreal averageValue() const;
  qreal distance() const;

  void setMaxValue(int newMaxValue);

  void startSimulation();

  void setInputStream(QTextStream &inputStream);
  void setInputDevice(QIODevice &device);
  // Need to set input from QML too. Just can't instantiate it there I think
  Q_INVOKABLE void setStandardInput();

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
  QIODevice *m_inputDevice;
  std::size_t m_time;
  QTimer *m_valueTimer;

  // Roughly copied from material
  void calculate_value();
  auto to_radians(double degrees);

  void updateStatistics();

  void readStream();
  void readDevice();
  QList<qreal> m_valueArray;
  qreal m_averageValue;
  qreal m_distance;
  int m_maxValue;
};

#endif // SPEEDGAUGECONTROL_H
