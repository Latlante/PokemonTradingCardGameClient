import QtQuick 2.0
import model 1.0

Item {
    id: popupListPlayers1
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

            Text {
                id: textNameGame
                height: 20
                text: qsTr("Nom de la partie")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                font.pixelSize: 20
            }

            Rectangle {
                id: rectangleContainerNameGame
                height: 25
                color: "#ffffff"
                border.width: 1
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: textNameGame.bottom
                anchors.topMargin: 0

                TextInput {
                    id: textInputNameGame
                    text: qsTr("Partie n1")
                    horizontalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                    font.pixelSize: 18
                }
            }

            Text {
                id: textOpponent
                text: qsTr("Adversaire")
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: rectangleContainerNameGame.bottom
                anchors.topMargin: 10
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 20
            }

            ListView {
                id: listViewAllPlayers
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: boutonOk.top
                anchors.bottomMargin: 0
                anchors.top: textOpponent.bottom
                anchors.topMargin: 0

                model: ctrlGameBoard.modelAllPlayers()
                delegate: Rectangle {
                    width: parent.width
                    height: 30
                    color: index == listViewAllPlayers.currentIndex ? "lightgray" : "white"

                    Text {
                        id: textNamePlayer
                        anchors.fill: parent
                        font.pixelSize: 20
                        color: "black"
                        text: model.name

                        MouseArea {
                            id: mouseAreaNamePlayer
                            anchors.fill: parent
                            onClicked: listViewAllPlayers.currentIndex = index
                        }
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

                onClicked: ctrlGameBoard.createANewGame(textInputNameGame.text, listViewAllPlayers.currentIndex+1)
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
