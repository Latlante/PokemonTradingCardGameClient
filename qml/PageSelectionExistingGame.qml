import QtQuick 2.0
import model 1.0

Item {
    id: pageSelectionExistingGame1
    width: 720
    height: 480

    Rectangle {
        id: background
        anchors.fill: parent

        color: "#2b95ff"

        Rectangle {
            id: rectangleContainerListView
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            anchors.fill: parent
            anchors.margins: 0

            ListView {
                id: listViewAllGamesAvailable
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: boutonOk.top
                anchors.bottomMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0

                model: ctrlGameBoard.modelAllOfGamesAvailable()
                delegate: Rectangle {
                    width: parent.width
                    height: 30

                    Text {
                        id: textNamePlayer
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: textRightArrow.left
                        font.pixelSize: 20
                        color: "black"
                        text: model.nameGame + "(" + model.nameOpponent + ")";
                    }

                    Text {
                        id: textRightArrow
                        width: parent.height
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        font.pixelSize: 28
                        color: "black"
                        text: ">"
                    }
                }
            }

            ButtonStyleGold {
                id: boutonOk

                width: parent.width / 2
                height: 40
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                text: qsTr("Ok")

                onClicked: ctrlGameBoard.joinAGame(listViewAllGamesAvailable.currentIndex)
            }




            ButtonStyleGold {
                id: boutonCancel

                height: 40
                anchors.left: boutonOk.right
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.bottom: parent.bottom
                text: qsTr("Cancel")

                onClicked: ctrlGameBoard.returnToTheMenu()
            }



        }



    }
}
