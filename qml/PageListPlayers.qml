import QtQuick 2.0
import model 1.0

Item {
    id: popupListPlayers1
    width: 720
    height: 480

    Rectangle {
        id: background
        anchors.fill: parent

        color: "#AAAAAA"

        Rectangle {
            id: rectangleContainerListView
            anchors.fill: parent
            anchors.margins: 0

            ListView {
                id: listViewAllPlayers
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: boutonOk.top
                anchors.bottomMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0

            }

            Rectangle {
                id: boutonOk

                width: parent.width / 2
                height: 40
                border.width: 1
                anchors.bottom: parent.bottom
                anchors.left: parent.left

                Text {
                    id: textBtOk
                    text: qsTr("Ok")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                    font.pixelSize: 20

                    MouseArea {
                        id: mouseAreaBtOk
                        anchors.fill: parent
                    }
                }


            }

            Rectangle {
                id: boutonCancel
                y: -6
                height: 40
                border.width: 1
                anchors.left: boutonOk.right
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.bottom: parent.bottom
                Text {
                    id: textBtCancel
                    text: qsTr("Annuler")
                    verticalAlignment: Text.AlignVCenter
                    MouseArea {
                        id: mouseAreaBtCancel
                        anchors.fill: parent
                    }
                    horizontalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                    font.pixelSize: 20
                }
            }
        }



    }
}
