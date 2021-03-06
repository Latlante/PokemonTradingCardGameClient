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
            anchors.topMargin: 5

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

        Rectangle {
            id: rectangleContainerListView
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            anchors.top: textOpponent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            color: "transparent"
            border.color: "black"
            border.width: 1


            ListView {
                id: listViewAllPlayers
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: buttonOk.top
                anchors.bottomMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0

                model: ctrlGameBoard.modelAllPlayersWithoutCurrentPlayer()
                delegate: Item {
                    width: parent.width
                    height: 30

                    Rectangle {
                        id: rectangleContainerDelegate
                        anchors.fill: parent
                        //color: "#ffc90e"
                        color: listViewAllPlayers.currentIndex == index ? "white" : "#60ffffff"
                        //border.color: "#0000ff"
                        border.color: "black"
                        border.width: 2

                        Text {
                            id: textDelegateInfo
                            //anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            anchors.right: parent.right
                            font.pixelSize: 14
                            font.bold: true
                            color: "black"
                            text: model.name;
                        }

                        MouseArea {
                            id: mouseAreaDelegateNamePlayer
                            anchors.fill: parent
                            onClicked: listViewAllPlayers.currentIndex = index
                        }
                    }
                }
            }

            Text {
                id: textError
                anchors.bottom: buttonOk.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 30
                color: "#ff0000"
                visible: ctrlGameBoard.error != ""
                text: ctrlGameBoard.error
            }

            ButtonStyleGold {
                id: buttonOk

                width: parent.width / 2
                height: 40
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                text: qsTr("Ok")

                onClicked: {
                    var realCurrentIndex = ctrlGameBoard.modelAllPlayersWithoutCurrentPlayer().mapIndexToSource(listViewAllPlayers.currentIndex)
                    ctrlGameBoard.createANewGame(textInputNameGame.text, realCurrentIndex+1)
                }
            }


            ButtonStyleGold {
                id: buttonCancel

                height: 40
                anchors.left: buttonOk.right
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
