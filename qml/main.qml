import QtQuick 2.9
import QtQuick.Controls 2.2
//import QtQuick.VirtualKeyboard 2.2
import model 1.0

ApplicationWindow {
    id: window1
    visible: true
    width: 1200
    height: 1920
    title: qsTr("Trading Card Game")

    property FactoryMainPageLoader factory: ctrlGameBoard.factory()

    Loader {
        id: loader1
        anchors.fill: parent

        source: factory.link
    }
}
