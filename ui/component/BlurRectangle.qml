import QtQuick 2.15
import Qt5Compat.GraphicalEffects

Rectangle {
    id: control
    property Item target
    property real noiseOpacity: 0.01
    property int blurRadius: 40
    clip: true
    FastBlur {
        id: blur
        source: control.target
        width: source.width
        height: source.height
        radius: control.blurRadius
    }

    Image {
        anchors.fill: parent
        source: "qrc:/res/img/noise.png"
        fillMode: Image.Tile
        opacity: control.noiseOpacity
    }

    onXChanged: setBlurPosition()
    onYChanged: setBlurPosition()
    Component.onCompleted: setBlurPosition()
    function setBlurPosition() {
        blur.x = target.x - x
        blur.y = target.y - y
    }
}
