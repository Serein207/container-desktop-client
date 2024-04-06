import QtQuick 2.15
import QtQuick.Layouts
import container_desktop
import "../component"
import "../item"

ScrollablePage {
    id: page
    function getCurrentTime() {
        var currentDate = new Date()
        var year = currentDate.getFullYear()
        var month = ("0" + (currentDate.getMonth() + 1)).slice(-2)
        var day = ("0" + currentDate.getDate()).slice(-2)
        var hours = ("0" + currentDate.getHours()).slice(-2)
        var minutes = ("0" + currentDate.getMinutes()).slice(-2)

        return year + "/" + month + "/" + day + " " + hours + ":" + minutes
    }
    Area {
        Layout.fillWidth: true
        height: 190
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
            id: text_time
            font.pointSize: 12
            color: "#718096"
            anchors {
                top: parent.top
                right: parent.right
            }
        }

        Rectangle {
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
                spacing: (parent.width - 340) / 5
                Column {
                    spacing: 10
                    Text {
                        id: container_num
                        font.pointSize: 20
                        font.bold: true
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
                        id: running_num
                        font.pointSize: 20
                        font.bold: true
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
                        text: PanelViewModel.curMemory
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        font.pointSize: 14
                        color: "#718096"
                        text: "已用内存"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
                Column {
                    spacing: 10
                    Text {
                        font.pointSize: 20
                        font.bold: true
                        text: PanelViewModel.curNetIn
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        font.pointSize: 14
                        color: "#718096"
                        text: "下载速度"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
                Column {
                    spacing: 10
                    Text {
                        font.pointSize: 20
                        font.bold: true
                        text: PanelViewModel.curCpu
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        font.pointSize: 14
                        color: "#718096"
                        text: "CPU占用"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
    }
    Row {
        Layout.alignment: Qt.AlignRight
        Layout.topMargin: 20
        spacing: 20
        ComboBox {
            id: comboBox_vm
            model: ContainerBlockViewModel
            textRole: "name"
            currentIndex: 0
            width: 200
            Shadow {
                radius: 10
            }
        }
        ComboBox {
            id: comboBox_timeFrame
            model: ["hour", "day", "week", "month", "year"]
            currentIndex: 0
            width: 200
            Shadow {
                radius: 10
            }
        }
        RaisedButton {
            text: "查询"
            radius: 10
            height: 50
            Shadow {
                radius: 10
            }
            onClicked: {
                showLoadingView()
                PanelViewModel.load(comboBox_vm.currentValue.vmId,
                                    comboBox_timeFrame.currentText)
            }
        }
    }

    Connections {
        target: PanelViewModel
        function onLoadSuccess() {
            text_time.text = "截止至 " + getCurrentTime()
            container_num.text = ContainerBlockViewModel.rowCount()
            running_num.text = ContainerBlockViewModel.getRunningVmNum()
            page.labels = JSON.parse(PanelViewModel.arrayTime)
            netIn.data = JSON.parse(PanelViewModel.arrayNetIn)
            netOut.data = JSON.parse(PanelViewModel.arrayNetOut)
            cpu.data = JSON.parse(PanelViewModel.arrayCpu)
            memory.data = JSON.parse(PanelViewModel.arrayMemory)
            showSuccessView()
        }
    }

    property var labels

    onErrorClicked: {
        showLoadingView()
        PanelViewModel.load(comboBox_vm.currentValue.vmId,
                            comboBox_timeFrame.currentText)
    }

    Connections {
        target: PanelViewModel
        function onLoadFailed(message) {
            showErrorView()
            showError("Error:" + message, 4000)
        }
    }

    SingleLineChart {
        id: memory
        Layout.fillWidth: true
        Layout.topMargin: 20
        title: "内存使用率"
        currentData: PanelViewModel.curMemory
        labels: page.labels
        borderColor: 'rgb(75, 192, 192)'
        backgroundColor: 'rgba(75, 192, 192, 0.5)'
    }

    SingleLineChart {
        id: cpu
        Layout.fillWidth: true
        Layout.topMargin: 20
        title: "CPU占用"
        currentData: PanelViewModel.curCpu
        labels: page.labels
        borderColor: 'rgb(61,174,233)'
        backgroundColor: 'rgba(61,174,233, 0.5)'
    }

    SingleLineChart {
        id: netIn
        Layout.fillWidth: true
        Layout.topMargin: 20
        title: "下载速度"
        currentData: PanelViewModel.curNetIn
        labels: page.labels
        borderColor: 'rgb(77,61,233)'
        backgroundColor: 'rgba(77,61,233, 0.5)'
    }

    SingleLineChart {
        id: netOut
        Layout.fillWidth: true
        Layout.topMargin: 20
        title: "上传速度"
        currentData: PanelViewModel.curNetOut
        labels: page.labels
        borderColor: 'rgb(206,61,233)'
        backgroundColor: 'rgba(206,61,233, 0.5)'
    }
}
