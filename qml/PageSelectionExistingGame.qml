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

        Text {
            id: textTheme
            height: 40
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            font.bold: true
            font.pixelSize: 29
            font.letterSpacing: 0
            text: "Sélectionner votre partie"
        }

        Rectangle {
            id: rectangleContainerListView
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            anchors.top: textTheme.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            color: "transparent"
            border.color: black
            border.width: 1

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
                delegate: Item {
                    width: parent.width
                    height: 30

                    Rectangle {
                        id: rectangleContainerDelegate
                        anchors.fill: parent
                        //color: "#ffc90e"
                        color: listViewAllGamesAvailable.currentIndex == index ? "white" : "#60ffffff"
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
                            text: model.nameGame + " (vs " + model.nameOpponent + ")";
                        }

                        Text {
                            id: textDelegateRightArrow
                            width: parent.height
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.right: parent.right
                            font.pixelSize: 28
                            color: "black"
                            text: ">"
                        }

                        MouseArea {
                            id: mouseAreaDelegateNameGame
                            anchors.fill: parent
                            onClicked: listViewAllGamesAvailable.currentIndex = index
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

                onClicked: ctrlGameBoard.joinAGame(listViewAllGamesAvailable.model.id(listViewAllGamesAvailable.currentIndex),
                                                   listViewAllGamesAvailable.model.name(listViewAllGamesAvailable.currentIndex),
                                                   listViewAllGamesAvailable.model.opponent(listViewAllGamesAvailable.currentIndex))

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
