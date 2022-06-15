// Histogram-component designed for a list of integers

import QtQuick 2.0

Rectangle {
  id: root
  border.color: "black"
  border.width: 1
  required property var sourceArray
  width: sourceArray.length * 2
  height: 200
  Row {
    id: histogram
    anchors.fill: parent
    property real maxValue: Math.max(...sourceArray)
    Repeater {
      id: barRepeater
      model: sourceArray.length
      Rectangle {
        anchors.bottom: histogram.bottom
        width: Math.round(root.width/sourceArray.length)
        color: "lightblue"
        height: 1
      }
    }
  }
  // Bind sourceArray to columns
  onSourceArrayChanged: {
    histogram.maxValue = Math.max(...sourceArray);
    for (var i = 0; i < sourceArray.length; i++) {
      // don't divide by zero
      if (sourceArray[i] !== 0 && histogram.maxValue !== 0) {
        barRepeater.itemAt(i).height = (sourceArray[i] / histogram.maxValue) * root.height;
      }
    }
  }

  Component.onCompleted: {
    console.log("Loaded histogram, arrlen: " + sourceArray.length)
  }
}
