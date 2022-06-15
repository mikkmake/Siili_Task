import QtQuick 2.0
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
        maxValue: 191
      }
    }
  }
  // Statistics histogram
  Window {
    id: statisticsWindow
    width: speedHistogram.width
    height: speedHistogram.height
    visible: true
    Histogram {
      id: speedHistogram
      sourceArray: speedControl.valueArray
      anchors.centerIn: parent
    }
  }
}
