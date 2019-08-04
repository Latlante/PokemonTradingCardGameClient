import QtQuick 2.0
import model 1.0
import "qml/BasicComponents"

Item {
    height: 600

    property bool stepInProgress: ctrlAnimation.stepInProgress
    property ModelListOfGamesAvailable modelGamesAvailable: ctrlGameBoard.modelAllOfGamesAvailable()
    property int numberOfGamesAvailable: modelGamesAvailable.count

    onStepInProgressChanged: {
        if(stepInProgress == false)
        {
            column1.visible = true
            loading_P1.visible = false
        }
        else if(stepInProgress == true)
        {
            column1.visible = false
            loading_P1.visible = true
        }
    }

    Rectangle {
        id: rectangleBackground
        color: "#2b95ff"
        anchors.fill: parent

        Image {
            id: imageLogo
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 0.3 * parent.width
            //anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectFit
            source: "Images/logo.png"
        }

        Text {
            id: textWelcome
            anchors.top: imageLogo.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.right: parent.right
            color: "#ffffff"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 28
            text: "Bienvenue " + ctrlGameBoard.modelAllPlayers().currentPlayer
        }

        Rectangle {
            id: rectangleContainerCreateChooseGame
            x: 204
            y: 202
            width: 300
            height: 244
            color: "#00000000"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 50
            border.width: 1

            Column {
                id: column1
                y: 70
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.rightMargin: 10
                anchors.leftMargin: 10
                spacing: 20

                Text {
                    id: textCreateChooseGame
                    width: parent.width
                    height: 30
                    color: "#ffffff"
                    text: qsTr("Gestion des parties")
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 20
                }



                Rectangle {
                    id: buttonCreateNewGame
                    width: 181
                    height: 50
                    color: "#ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter

                    Text {
                        id: textButtonCreateNewGame
                        text: qsTr("Créer une nouvelle partie")
                        wrapMode: Text.WordWrap
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20

                        MouseArea {
                            id: mouseAreaButtonCreateNewGame
                            anchors.fill: parent

                            onClicked: {
                                buttonCreateNewGame.enabled = false
                                ctrlGameBoard.listOfAllPlayers()
                            }
                        }
                    }
                }

                Rectangle {
                    id: buttonJoinGame
                    width: 181
                    height: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#ffffff"
                    enabled: numberOfGamesAvailable > 0

                    Text {
                        id: textButtonJoinGame
                        text: qsTr("Rejoindre une partie")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.fill: parent
                        font.pixelSize: 20
                        wrapMode: Text.WordWrap

                        MouseArea {
                            id: mouseAreaButtonJoinGame
                            anchors.fill: parent
                            onClicked: {
                                ctrlGameBoard.listOfGamesAvailable()
                            }
                        }
                    }
                }

                Rectangle {
                    id: rectangleContainerGameInProgress
                    width: parent.width
                    height: 30
                    color: "transparent"

                    Text {
                        id: textGameInProgress
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: imageButtonRefresh.left
                        anchors.bottom: parent.bottom
                        color: "#ffffff"
                        text: qsTr("Parties en cours: ") + numberOfGamesAvailable
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 16
                        font.bold: true
                    }

                    Image {
                        id: imageButtonRefresh
                        width: parent.height
                        height: parent.height
                        anchors.top: parent.top
                        anchors.right: parent.right
                        source: "qrc:/Images/refresh-arrows.png"

                        MouseArea {
                            id: mouseAreaButtonRefresh
                            anchors.fill: parent
                            onClicked: ctrlGameBoard.listOfGamesAlreadyExisting()
                        }
                    }

                    /*Rectangle {
                        id: rectangleContainerButtonRefresh
                        width: parent.height
                        height: parent.height
                        anchors.top: parent.top
                        anchors.right: parent.right
                        color: "red"

                        MouseArea {
                            id: mouseAreaButtonRefresh
                            anchors.fill: parent
                            onClicked: ctrlGameBoard.listOfGamesAlreadyExisting()
                        }
                    }*/
                }
            }

            Loading_P {
                id: loading_P1
                width: 100
                height: 100
                visible: false
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

}
