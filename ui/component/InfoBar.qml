import QtQuick 2.15
import QtQuick.Controls

MaterialObject {
    property var root
    property int layoutY: 75
    id: control
    MaterialObject {
        id: mcontrol
        property string const_success: "success"
        property string const_info: "info"
        property string const_warning: "warning"
        property string const_error: "error"
        property int maxWidth: 300
        property var screenLayout: null
        function create(type, text, duration) {
            if (screenLayout) {
                var last = screenLayout.getLastloader()
                if (last.type === type && last.text === text) {
                    last.restart()
                    return
                }
            }
            initScreenLayout()
            contentComponent.createObject(screenLayout, {
                                              "type": type,
                                              "text": text,
                                              "duration": duration
                                          })
        }
        function createCustom(itemcomponent, duration) {
            initScreenLayout()
            if (itemcomponent) {
                contentComponent.createObject(screenLayout, {
                                                  "itemcomponent": itemcomponent,
                                                  "duration": duration
                                              })
            }
        }
        function initScreenLayout() {
            if (screenLayout == null) {
                screenLayout = screenlayoutComponent.createObject(root)
                screenLayout.y = control.layoutY
                screenLayout.z = 100000
            }
        }
        Component {
            id: screenlayoutComponent
            Column {
                spacing: 20
                width: parent.width
                move: Transition {
                    NumberAnimation {
                        properties: "y"
                        easing.type: Easing.OutCubic
                        duration: 333
                    }
                }
                onChildrenChanged: if (children.length === 0)
                                       destroy()
                function getLastloader() {
                    if (children.length > 0) {
                        return children[children.length - 1]
                    }
                    return null
                }
            }
        }
        Component {
            id: contentComponent
            Item {
                id: content
                property int duration: 1500
                property var itemcomponent
                property string type
                property string text
                width: parent.width
                height: loader.height
                function close() {
                    content.destroy()
                }
                function restart() {
                    delayTimer.restart()
                }
                Timer {
                    id: delayTimer
                    interval: duration
                    running: duration > 0
                    repeat: duration > 0
                    onTriggered: content.close()
                }
                Loader {
                    id: loader
                    x: (parent.width - width) / 2
                    property var _super: content
                    scale: item ? 1 : 0
                    asynchronous: true
                    Behavior on scale {
                        NumberAnimation {
                            easing.type: Easing.OutCubic
                            duration: 167
                        }
                    }
                    sourceComponent: itemcomponent ? itemcomponent : mcontrol.fluent_sytle
                    Component.onDestruction: sourceComponent = undefined
                }
            }
        }
        property Component fluent_sytle: Rectangle {
            width: text.width + 20
            height: text.height + 20
            color: {
                switch (_super.type) {
                case mcontrol.const_success:
                    return "#dff6dd"
                case mcontrol.const_warning:
                    return "#fff4ce"
                case mcontrol.const_info:
                    return "#f4f4f4"
                case mcontrol.const_error:
                    return "#fde7e9"
                }
                return "#FFFFFF"
            }
            Shadow {
                radius: 4
            }
            radius: 4
            border.width: 1
            border.color: {
                switch (_super.type) {
                case mcontrol.const_success:
                    return "#d2e8d0"
                case mcontrol.const_warning:
                    return "#f0e6c2"
                case mcontrol.const_info:
                    return "#e6e6e6"
                case mcontrol.const_error:
                    return "#eed9db"
                }
                return "#FFFFFF"
            }
            Text {
                id: text
                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                text: _super.text
                wrapMode: Text.WrapAnywhere
                width: Math.min(implicitWidth, mcontrol.maxWidth)
            }
        }
    }
    function showSuccess(text, duration = 1000) {
        mcontrol.create(mcontrol.const_success, text, duration)
    }
    function showInfo(text, duration = 1000) {
        mcontrol.create(mcontrol.const_info, text, duration)
    }
    function showWarning(text, duration = 1000) {
        mcontrol.create(mcontrol.const_warning, text, duration)
    }
    function showError(text, duration = 1000) {
        mcontrol.create(mcontrol.const_error, text, duration)
    }
    function showCustom(itemcomponent, duration = 1000) {
        mcontrol.createCustom(itemcomponent, duration)
    }
}
