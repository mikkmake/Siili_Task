import QtQuick 2.0
import Qt.labs.animation
import GaugeControl

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
      maxValue: 190
    }
  }
}
