import QtQuick 2.15
import QtQuick.Controls
import "../page"

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
}
