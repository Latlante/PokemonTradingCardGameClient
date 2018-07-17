import QtQuick 2.0

Item {
    height: 600

    Rectangle {
        id: rectangleBackground
        color: "#2b95ff"
        anchors.fill: parent

        Image {
            id: imageLogo
            x: 0
            y: 0
            width: parent.width
            height: 0.3 * parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectFit
            source: "Images/logo.png"
        }

        Rectangle {
            id: rectangleContainerCreateChooseGame
            x: 204
            y: 202
            width: 254
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

                            }
                        }
                    }
                }


                Text {
                    id: textGameInProgress
                    width: parent.width
                    height: 30
                    color: "#ffffff"
                    text: qsTr("Parties en cours: 1")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 16
                    font.bold: true
                }
            }
        }
    }

}
