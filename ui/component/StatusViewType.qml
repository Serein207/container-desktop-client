pragma Singleton

import QtQuick 2.15

QtObject {
    readonly property int success: 0
    readonly property int loading: 1
    readonly property int error: 2
    readonly property int empty: 3
}
