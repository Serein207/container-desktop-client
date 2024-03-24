import QtQuick 2.15
import QtQuick.Layouts
import container_desktop
import "../item"
import "../component"

ScrollablePage {
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
            name: model.name
            source: "qrc:/res/img/kylinOS.png"
            info: model.vmId
            cpu: model.cpu
            memory: model.mem
            status: model.status
            remark: model.remark
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

    onErrorClicked: {
        showLoadingView()
        ContainerBlockViewModel.load()
    }
}
