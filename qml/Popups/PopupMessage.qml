import QtQuick 2.0
import model 1.0

Item {
    id: popupCardDetailBasic1
    width: 1000
    height: 500

    Rectangle {
        id: background
        anchors.fill: parent

        color: "#AAAAAAAA"

        Text {
            id: textMessage
            anchors.fill: parent
            anchors.margins: 0.25 * parent.width
            font.pixelSize: 30
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: ctrlPopups.messageContent
        }

        MouseArea {
            id: mouseAreaClosing
            anchors.fill: parent
            onClicked: ctrlPopups.selectionCardsFinished()
        }
    }

}
