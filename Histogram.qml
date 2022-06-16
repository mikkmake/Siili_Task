// Histogram-component designed for a list of integers
// Displays a column for each value in sourceArray, treating array length as the X-axis

import QtQuick 2.0

Rectangle {
  id: root
  border.color: "black"
  border.width: 1
  required property var sourceGaugeControl
  property int value: sourceGaugeControl.value
  property var sourceArray: sourceGaugeControl.valueArray
  required property string titleText
  property string xLabelText
  property string yLabelText
  width: histogramBox.width + 100
  height: histogramBox.height + 100

  // Rudimentary optional labels
  Text {
    id: title
    text: titleText
    anchors {
      top: parent.top
      horizontalCenter: parent.horizontalCenter
      topMargin: 25
    }
  }
  Text {
    id: xLabel
    text: xLabelText
    anchors {
      horizontalCenter:parent.horizontalCenter
      bottom: parent.bottom
    }
  }
  Text {
    id: yLabel
    text: yLabelText
    rotation: -90
    anchors {
      verticalCenter: parent.verticalCenter
      left: parent.left
      leftMargin: -15
    }
  }
  Text {
    id: xMaxValue
    text: sourceGaugeControl.valueArray.length -1
    anchors {
      right: parent.right
      bottom: parent.bottom
      rightMargin: 40
      bottomMargin: 30
    }
  }
  Text {
    id: xHalfValue
    text: sourceGaugeControl.valueArray.length / 2 - 1
    anchors {
      horizontalCenter: parent.horizontalCenter
      bottom: parent.bottom
      bottomMargin: 30
    }
  }
  Text {
    id: yMaxValue
    text: histogram.maxValue.toFixed(2)
    anchors {
      top: parent.top
      left: parent.left
      topMargin: 45
      leftMargin: 20
    }
  }
  Text {
    id: yHalfValue
    text: (histogram.maxValue / 2).toFixed(2)
    anchors {
      verticalCenter: parent.verticalCenter
      left: parent.left
      leftMargin: 20
    }
  }

  // Histogram container
  Rectangle {
    id: histogramBox
    width: sourceGaugeControl.valueArray.length * 2 + border.width * 2
    height: 200
    border.color: "black"
    border.width: 1
    anchors.centerIn: parent

    Row {
      id: histogram
      anchors.fill: parent
      anchors.margins: histogramBox.border.width
      property real maxValue: 0.5 // Init to prevent division by zero later

      Repeater {
        id: barRepeater
        model: sourceGaugeControl.valueArray.length
        Rectangle {
          anchors.bottom: histogram.bottom
          width: Math.round(histogramBox.width/sourceGaugeControl.valueArray.length)
          color: "lightblue"
          height: 0.1 // Initial height needed to have 0-valued columns placed in too
        }
      }
    }
  }

  // Bind sourceArray to columns
  // onSourceArrayChanged: {
    // histogram.maxValue = Math.max(...sourceArray);
    // // Begin from one as we don't want to display time spent at zero
    // for (var i = 1; i < sourceArray.length; i++) {
      // // don't divide by zero
      // if (sourceArray[i] !== 0 && histogram.maxValue !== 0) {
        // barRepeater.itemAt(i).height = ((sourceArray[i] / histogram.maxValue) * histogramBox.height) - histogramBox.border.width * 2;
      // }
    // }
  // }

  // Show changes in value
  onSourceArrayChanged: {
    if (sourceGaugeControl.valueArray[value] > histogram.maxValue)
       histogram.maxValue = sourceGaugeControl.valueArray[value];
    barRepeater.itemAt(value).height = ((sourceGaugeControl.valueArray[value] / histogram.maxValue) * histogramBox.height);
  }
}
