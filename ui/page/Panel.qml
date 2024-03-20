import QtQuick 2.15
import QtQuick.Layouts
import "../component"

ScrollablePage {
    Area {
        Layout.fillWidth: true
        height: 320
        paddings: 20
        Text {
            id: text_title
            text: "实时数据"
            font.pointSize: 16
            anchors {
                top: parent.top
                left: parent.left
            }
        }
        Text {
            text: "截止至 2022/06/30 12:00"
            font.pointSize: 12
            color: "#718096"
            anchors {
                top: parent.top
                right: parent.right
            }
        }

        Rectangle {
            id: rec
            height: 93
            color: "#f3f3f3"
            radius: 8
            anchors {
                top: text_title.bottom
                topMargin: 20
                left: parent.left
                right: parent.right
            }
            Row {
                id: row
                anchors.centerIn: parent
                spacing: (parent.width - 340) / 3
                Column {
                    spacing: 10
                    Text {
                        font.pointSize: 20
                        font.bold: true
                        text: "13"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        font.pointSize: 14
                        color: "#718096"
                        text: "总计容器"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
                Column {
                    spacing: 10
                    Text {
                        font.pointSize: 20
                        font.bold: true
                        text: "6"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        font.pointSize: 14
                        color: "#718096"
                        text: "运行中"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
                Column {
                    spacing: 10
                    Text {
                        font.pointSize: 20
                        font.bold: true
                        text: "2"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        font.pointSize: 14
                        color: "#718096"
                        text: "镜像"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
                Column {
                    spacing: 10
                    Text {
                        font.pointSize: 20
                        font.bold: true
                        text: "6"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        font.pointSize: 14
                        color: "#718096"
                        text: "数据卷"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
        Text {
            id: text1
            text: "内存使用率"
            font.pointSize: 14
            color: "#718096"
            anchors {
                top: rec.bottom
                topMargin: 20
                left: parent.left
            }
        }
        Text {
            id: text2
            text: "73%"
            font.pointSize: 28
            font.bold: true
            anchors {
                horizontalCenter: text1.horizontalCenter
                top: text1.bottom
                topMargin: 5
            }
        }
        Text {
            text: "建议扩容"
            font.pointSize: 12
            color: "#718096"
            anchors {
                horizontalCenter: text1.horizontalCenter
                top: text2.bottom
                topMargin: 5
            }
        }
        Chart {
            anchors {
                left: text1.right
                leftMargin: 20
                right: parent.right
                top: rec.bottom
                topMargin: 15
                bottom: parent.bottom
            }
            chartType: 'line'
            chartData: {
                return {
                    "labels": ['January', 'February', 'March', 'April', 'May', 'June', 'July'],
                    "datasets": [{
                            "label": 'My First Dataset',
                            "data": [65, 59, 80, 81, 56, 55, 40],
                            "fill": true,
                            "borderColor": 'rgb(75, 192, 192)',
                            "tension": 0.1
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
                    }
                }
            }
        }
    }
}
