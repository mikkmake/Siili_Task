import QtQuick
import QtQuick.Controls
import Qt.labs.animation
import GaugeControl

Item {
  id: root
  height: background.height
  width: background.width
  Image {
    id: background
    source: "assets/svgBackgroundCircle.png"

    Image {
      id: topCircle
      source: "assets/TopCircle.png"
      anchors.centerIn: parent
      Text {
        id: currentSpeed
        text: speedControl.value
        color: "white"
        font.pixelSize: 32
        anchors.centerIn: parent
      }
      Text {
        id: distance
        text: (speedControl.distance / 1000).toFixed(0) + " km"
        color: "white"
        font.pixelSize: 16
        anchors {
          horizontalCenter: parent.horizontalCenter
          bottom: parent.bottom
          bottomMargin: 50
        }

      }
    }
    Image {
      id: speedNumbers
      source: "assets/Speed\ texture.png"
      anchors.centerIn: parent
    }
    Image {
      id: needle
      source: "assets/Needle.png"
      x: 24; y: 24
      rotation: -45 + 1.5 * speedControl.value // Begin from 0, then match speed-value
      BoundaryRule on rotation {
        minimum: -45
        maximum: 240
      }
      PropertyAnimation {
        target: needle
        property: "rotation"
        duration: 50
        running: true
      }
      GaugeControl {
        id: speedControl
        maxValue: 190
        Component.onCompleted: {
          setStandardInput()
        }
      }
    }
    Button {
      text: "Toggle Histogram"
      anchors {
        horizontalCenter: parent.horizontalCenter
        bottom: parent.bottom
      }
      onClicked: {
        statisticsWindow.visible = !statisticsWindow.visible
      }
    }
  }
  // Statistics histogram
  Window {
    id: statisticsWindow
    minimumWidth: speedHistogram.width
    minimumHeight: speedHistogram.height
    title: "SpeedGauge Histogram"
    visible: true
    Histogram {
      id: speedHistogram
      sourceGaugeControl: speedControl
      anchors.centerIn: parent
      xLabelText: "Speed (km/h)"
      yLabelText: "Time (s)"
      titleText: "Histogram: time spent at certain speeds"
    }
  }
}
