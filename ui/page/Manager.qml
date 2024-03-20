import QtQuick 2.15
import QtQuick.Layouts
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
        model: 12
        cellHeight: 250
        cellWidth: 380
        delegate: ContainerCard {
            name: "KylinOS"
            source: "qrc:/res/img/kylinOS.png"
            info: "3abf3s324"
            createTime: "2023-2-23"
            status: "运行中"
            leftMemory: "4GB"
            remark: "上课时用的系统"
        }
    }
}
