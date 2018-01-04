import QtQuick 2.0
import QtCharts 2.2

Item {
    id: root

    ChartView {
        id: chart
        width: root.height
        height: root.width

        title: ' '
        antialiasing: true
        legend.visible: false
        backgroundColor: 'transparent'
        plotAreaColor: 'transparent'
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        transform: Rotation { origin.x: height/2; origin.y: width/2; angle: 90}

        ValueAxis {
            id: axisX
            titleText: qsTr("Distance [m]")
            visible: true
            lineVisible: true
            gridVisible: true
            labelsAngle: -90
            labelsVisible: true
            labelFormat: "%.1f"
            //labelFormat: " "
            tickCount: 3
            min: 0
            max: 200
        }

        ValueAxis {
            id: axisY
            titleText: qsTr("Intensity [dB]")
            visible: true
            lineVisible: true
            gridVisible: true
            labelsAngle: -90
            labelsVisible: true
            labelFormat: "%.1f"
            //labelFormat: " "
            tickCount: 2
            min: -255
            max: 255
        }

        LineSeries {
            id: serie
            pointLabelsVisible: false
            pointsVisible: false
            axisX: axisX
            axisY: axisY
        }

        LineSeries {
            id: serieInv
            pointLabelsVisible: false
            pointsVisible: false
            axisX: axisX
            axisY: axisY
        }

        function draw(points) {
            serie.clear()
            serieInv.clear()

            if (points) {
                for (var i=0; points.length; i++) {
                    serie.append(i, points[i])
                    serieInv.append(i, points[i])
                }
            } else {
                for (var i=0; i<200; i++) {
                    var pt = 255*Math.random()
                    serie.append(i, pt)
                    serieInv.append(i, -pt)
                }
            }
        }

        Timer {
            interval: 100; running: true; repeat: true
            onTriggered: {
                chart.draw()
            }
        }
    }
}
