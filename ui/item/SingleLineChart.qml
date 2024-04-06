import QtQuick 2.15
import QtQuick.Layouts
import "../component"

Area {
    id: control
    height: 280
    paddings: 20
    property string title
    property alias currentData: text2.text
    property var labels
    property var data
    property string borderColor: 'rgb(75, 192, 192)'
    property string backgroundColor: 'rgba(75, 192, 192, 0.5)'

    Text {
        id: text1
        font.pointSize: 14
        text: control.title
        color: "#718096"
        anchors {
            top: parent.top
            topMargin: 10
            left: parent.left
        }
    }
    Text {
        id: text2
        font.pointSize: 20
        font.bold: true
        anchors {
            left: text1.right
            leftMargin: 10
            horizontalCenter: text1.horizontalCenter
            bottom: text1.bottom
        }
    }
    Chart {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: text2.bottom
            topMargin: 10
        }
        chartType: 'line'
        chartData: {
            return {
                "labels": control.labels,
                "datasets": [{
                        "label": control.title,
                        "data": control.data,
                        "fill": true,
                        "borderColor": control.borderColor,
                        "backgroundColor": control.backgroundColor,
                        "tension": 0.1,
                        "pointStyle": false,
                        "pointRadius": 0
                    }]
            }
        }
        chartOptions: {
            return {
                "legend": {
                    "display": false
                },
                "maintainAspectRatio": false,
                "title": {
                    "display": false
                },
                "tooltips": {
                    "mode": 'index',
                    "intersect": false
                },
                "scales": {
                    "y": {
                        "suggestedMin": 0,
                        "suggestedMax": 100
                    }
                }
            }
        }
    }
}
