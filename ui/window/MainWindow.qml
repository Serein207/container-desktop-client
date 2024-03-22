import QtQuick 2.15
import QtQuick.Controls
import "../page"
import "../component"

ApplicationWindow {
    id: window
    minimumWidth: 1280
    minimumHeight: 720
    width: minimumWidth
    height: minimumHeight
    visible: true
    title: "Container Desktop"

    Loader {
        id: loader
        sourceComponent: loginPage
    }
    Component {
        id: loginPage
        LoginPage {
            width: window.width
            height: window.height
        }
    }
    Component {
        id: mainPage
        MainPage {
            width: window.width
            height: window.height
        }
    }
    function enterMainPage() {
        loader.sourceComponent = mainPage
        loginPage.destroy()
    }

    InfoBar {
        id: infoBar
        root: window
    }
    function showSuccess(text, duration) {
        infoBar.showSuccess(text, duration)
    }
    function showInfo(text, duration) {
        infoBar.showInfo(text, duration)
    }
    function showWarning(text, duration) {
        infoBar.showWarning(text, duration)
    }
    function showError(text, duration) {
        infoBar.showError(text, duration)
    }
}
