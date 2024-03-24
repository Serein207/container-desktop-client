import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: control
    default property alias content: container.data
    property alias statusMode: status_view.statusMode
    property color color: "#fff"
    property int spacing: 0
    property int topMargin: 100
    property int leftRightMargin: 65
    property color activeColor: "#335eea"
    signal errorClicked
    opacity: visible
    StatusView {
        id: status_view
        Layout.fillWidth: true
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
            bottomMargin: 10
        }
        color: "#00000000"
        statusMode: StatusViewType.success
        onErrorClicked: control.errorClicked()

        Flickable {
            id: flickview
            clip: true
            anchors.fill: parent
            contentWidth: parent.width
            contentHeight: container.implicitHeight + control.topMargin
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
                anchors {
                    right: flickview.right
                    top: parent.top
                    topMargin: 65
                }
            }
            boundsBehavior: Flickable.StopAtBounds
            ColumnLayout {
                id: container
                spacing: control.spacing
                anchors {
                    fill: parent
                    topMargin: control.topMargin
                    bottomMargin: control.topMargin
                    leftMargin: control.leftRightMargin
                    rightMargin: control.leftRightMargin
                }
                width: parent.width
            }
        }
    }
    function showSuccessView() {
        status_view.endLoading()
        statusMode = StatusViewType.success
    }
    function showLoadingView() {
        statusMode = StatusViewType.loading
        status_view.startLoading()
    }
    function showEmptyView() {
        statusMode = StatusViewType.empty
    }
    function showErrorView() {
        statusMode = StatusViewType.error
    }
}
