import QtQuick 2.0
import model 1.0
import "../BasicComponents"

Item {
    id: test_RotationCoin1
    width: 720
    height: 480

    Rectangle {
        id: rectangleContainerButtonBack
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 40
        color: "#AAAAAA"

        Text {
            id: textButtonBack
            anchors.fill: parent
            text: "Back"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        MouseArea {
            id: mouseAreaButtonBack
            anchors.fill: parent
            onClicked: ctrlGameBoard.factory().displayMenuTest()
        }
    }

    Rectangle {
        id: background
        anchors.top: rectangleContainerButtonBack.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "#AAAAAA"

        RotationCoin {
            id: rotationCoin1
            width: 100
            height: 100
            visible: true
            x: parent.width / 2
            y: parent.height / 2
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.verticalCenter: parent.verticalCenter
        }
    }
}
