import QtQuick 2.15
import QtQuick.Layouts
import container_desktop
import "../item"
import "../component"
import "../window"

ScrollablePage {
    id: page
    GridView {
        id: grid_view
        interactive: false
        Layout.fillWidth: true
        implicitHeight: contentHeight
        implicitWidth: contentWidth
        width: implicitWidth
        height: implicitHeight
        model: ContainerBlockViewModel
        cellHeight: 260
        cellWidth: 380
        delegate: ContainerCard {
            id: card
            name: model.name
            source: "qrc:/res/img/kylinOS.png"
            info: model.vmId
            cpu: model.cpu
            memory: model.mem
            status: model.status
            remark: model.remark
            onStartClicked: {
                ContainerViewModel.load(vmId)
            }
            onShutdownClicked: {
                ContainerViewModel.close(vmId)
            }
            onMoreClicked: {
                dialog.vmId = model.vmId
                dialog.open()
            }
        }
    }

    ContentDialog {
        id: dialog
        title: "More"
        height: pivot.height + 60
        width: 1000
        x: (page.width - dialog.width) / 2 - 65
        property string vmId
        Pivot {
            id: pivot
            anchors {
                fill: parent
                topMargin: 60
                margins: 30
            }
            currentIndex: 0
            PivotItem {
                title: "快照管理"
                contentItem: Snapshot {
                    vmId: dialog.vmId
                    anchors.fill: parent
                }
            }
            PivotItem {
                title: "容器设置"
                contentItem: ContainerSettings {}
            }
            PivotItem {
                title: "远程桌面连接设置"
                contentItem: RemoteDesktop {
                    vmId: dialog.vmId
                }
            }
        }
    }

    Component.onCompleted: {
        showLoadingView()
        ContainerBlockViewModel.load()
    }

    Connections {
        target: ContainerBlockViewModel
        function onLoadSuccess() {
            showSuccessView()
        }
    }

    Connections {
        target: ContainerBlockViewModel
        function onLoadFailed(message) {
            showError("Error: " + message, 4000)
            showErrorView()
        }
    }

    Connections {
        target: ContainerViewModel
        function onLoadSuccess() {}
    }

    onErrorClicked: {
        showLoadingView()
        ContainerBlockViewModel.load()
    }
}
