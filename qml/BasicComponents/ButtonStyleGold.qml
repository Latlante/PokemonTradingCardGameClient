import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    id: buttonStyleGold
    width: 500
    height: 100

    property alias text: textButton.text
    signal clicked

    /*style: ButtonStyle {
        background: Rectangle {
            implicitWidth: 500
            implicitHeight: 100
            color: "#ffc90e"
        }

        label: Text {

        }
    }*/

    Rectangle {
        id: rectangleBackground
        property string backgroundNormal: "#ffc90e"
        property string backgroundPressed: "#ffe63c"

        anchors.fill: parent
        color: backgroundNormal
    }

    Text {
        id: textButton
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 0.5 * height
        text: "Button"
    }

    MouseArea {
        anchors.fill: parent

        onPressed: rectangleBackground.color = rectangleBackground.backgroundPressed
        onReleased: rectangleBackground.color = rectangleBackground.backgroundNormal

        onClicked: buttonStyleGold.clicked();
    }

}
