import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.12

/* Usage:
 ComboSlider {
        id: slider
        // not needed, used if it exists.
        model: [strings | numbers]
        onOptionChanged: {
            console.log("Value " + value + " at key " + key)
        }
    }
*/

PingSlider {
    id: root

    property alias model: internalTicks.model
    property var startingIndex: null
    property var startingValue: null
    signal optionChanged(var modelKey, var modelValue)

    valueText: model[value]
    value: 0

    control.from: 0
    control.to: model.length - 1
    control.stepSize: 1

    onValueChanged: optionChanged(value, model[value])

    function getNearIndexFromModel(testValue) {
        var closestValue = {difference: -1, index: 0}
        for(var i in root.model) {
            var diff = Math.abs(root.model[i] - testValue);
            if(closestValue.difference === -1 || diff < closestValue.difference) {
                closestValue.difference = diff
                closestValue.index = i
            }
        }
        if(closestValue.difference !== -1) {
            return closestValue.index
        } else {
            return -1;
        }
    }

    control.background: Rectangle {
        x: control.leftPadding
        y: control.topPadding + control.availableHeight/2 - height/2
        implicitWidth: 200
        implicitHeight: 1
        width: control.availableWidth
        height: implicitHeight
        color: Material.accent

        Repeater {
            id: internalTicks

            delegate: Rectangle {
                id: tick
                width: 1
                height: parent.implicitHeight * 6
                x: index*(control.availableWidth - control.handle.width)/control.to + control.handle.width/2
                y: parent.implicitHeight
                color: Material.accent

                Text {
                    anchors.top: tick.bottom
                    id: textDelegate
                    text: modelData
                    color: Material.accent
                    x: -width/2
                }
            }
        }
    }

    Component.onCompleted: {
        if (root.startingIndex !== null) {
            root.value = root.startingIndex;
        } else if (root.startingValue !== null) {
            root.value = getNearIndexFromModel(root.startingValue);
        }
    }
}


