import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    id: buttonStyleGold
    width: 500
    height: 100

    property alias text: textButton.text
    signal clicked

    Rectangle {
        id: rectangleBackground
        property string backgroundNormal: "#ffc90e"
        property string backgroundPressed: "#ffe63c"
        gradient: Gradient {
            GradientStop {
                position: 0.671
                color: "#ffc90e"
            }

            GradientStop {
                position: 1
                color: "#ffffff"
            }
        }

        anchors.fill: parent
        border.width: 2
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

    Rectangle {
        id: rectangle
        color: "#00000000"
        border.width: 1
        anchors.margins: 5
        anchors.fill: parent
    }

}
