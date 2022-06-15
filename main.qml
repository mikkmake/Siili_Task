import QtQuick 2.15
import QtQuick.Window 2.15

Window {
  minimumWidth: speedGauge.width
  minimumHeight: speedGauge.height
  visible: true
  title: qsTr("SpeedGauge Test")
  SpeedGauge {
    id: speedGauge
    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter
  }
}
