import QtQuick 2.15
import "../component"

ScrollablePage {

    property real progress: 0
    Component.onCompleted: {
        timer.start()
        showLoadingView()
    }
    Timer {
        id: timer
        interval: 100
        running: true
        repeat: true
        onTriggered: {
            progress = (progress + 0.1).toFixed(1)
            console.log(progress)
            if (progress === 2) {
                timer.stop()
                showSuccessView()
            }
        }
    }

    Text {
        text: "Container Manager"
    }
}
