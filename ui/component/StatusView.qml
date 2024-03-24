import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: control
    default property alias content: container.data
    property int statusMode: StatusViewType.loading
    property string loadingText: "Loading..."
    property string emptyText: "Nothing"
    property string errorText: "oops, something went wrong!"
    property string errorButtonText: "Reload"
    property color activeColor: "#335eea"
    property color color: Window.active ? Qt.rgba(
                                              38 / 255, 44 / 255, 54 / 255,
                                              1) : Qt.rgba(39 / 255, 39 / 255, 39 / 255, 1)
    property int topLoadingMargin: 60
    signal errorClicked
    property Component loadingItem: com_loading
    property Component emptyItem: com_empty
    property Component errorItem: com_error
    property real progress: 0

    Item {
        id: container
        anchors.fill: parent
        visible: statusMode === StatusViewType.success
    }
    Loader {
        id: loader
        anchors.fill: parent
        visible: statusMode !== StatusViewType.success
        sourceComponent: {
            if (statusMode === StatusViewType.loading) {
                return loadingItem
            }
            if (statusMode === StatusViewType.empty) {
                return emptyItem
            }
            if (statusMode === StatusViewType.error) {
                return errorItem
            }
            return undefined
        }
    }

    Timer {
        id: timer_progress
        interval: 100
        onTriggered: {
            control.progress = (control.progress + 0.1).toFixed(1)
            if (control.progress === 0.8)
                timer_progress.stop()
            else
                timer_progress.start()
            if (control.progress >= 1.4) {
                statusMode = StatusViewType.success
            }
        }
    }
    function startLoading() {
        control.progress = 0
        timer_progress.restart()
    }
    function endLoading() {
        timer_progress.start()
    }
    Component {
        id: com_loading
        Area {
            paddings: 0
            border.width: 0
            radius: 0
            color: control.color
            Layout.fillWidth: true
            ColumnLayout {
                anchors {
                    top: parent.top
                    topMargin: control.topLoadingMargin
                }
                Item {
                    id: clip
                    width: control.width
                    height: 4
                    Rectangle {
                        id: rect_back
                        width: clip.width * control.progress
                        height: control.progress === 1 ? 0 : clip.height
                        radius: 1
                        color: control.activeColor
                        anchors.bottom: clip.bottom
                        Behavior on height {
                            enabled: control.progress >= 1.0
                            SequentialAnimation {
                                PauseAnimation {
                                    duration: 100
                                }
                                NumberAnimation {
                                    duration: 100
                                    from: 4
                                    to: 0
                                }
                            }
                        }
                        Behavior on width {
                            NumberAnimation {
                                duration: 167
                            }
                        }
                    }
                }
            }
        }
    }
    Component {
        id: com_empty
        Area {
            paddings: 0
            border.width: 0
            radius: 0
            color: control.color
            ColumnLayout {
                anchors.centerIn: parent
                Text {
                    text: control.emptyText
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }
    }
    Component {
        id: com_error
        Area {
            paddings: 0
            border.width: 0
            radius: 0
            color: control.color
            ColumnLayout {
                anchors.centerIn: parent
                Text {
                    text: control.errorText
                    Layout.alignment: Qt.AlignHCenter
                }
                FlatButton {
                    id: btn_error
                    upper: false
                    Layout.alignment: Qt.AlignHCenter
                    text: control.errorButtonText
                    onClicked: {
                        control.errorClicked()
                    }
                }
            }
        }
    }
}
