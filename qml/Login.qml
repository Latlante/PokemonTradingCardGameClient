import QtQuick 2.0


Item {
    height: 600
    Rectangle {
        id: rectangleBackground
        color: "#2b95ff"
        anchors.fill: parent

        Image {
            id: imageLogo
            x: 93
            width: parent.width
            height: 0.3 * parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 0
            fillMode: Image.PreserveAspectFit
            source: "Images/logo.png"
        }

        Rectangle {
            id: rectangle
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
                spacing: 5

                Text {
                    id: textAuthentification
                    width: parent.width
                    height: 30
                    color: "#ffffff"
                    text: qsTr("Authentification")
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 20
                }

                Text {
                    id: text1
                    color: "#ffffff"
                    text: qsTr("Nom")
                    font.pixelSize: 20
                }

                Rectangle {
                    id: rectangleContainerName
                    width: parent.width
                    height: 30
                    color: "#ffffff"
                    border.width: 1

                    TextEdit {
                        id: textEditName
                        text: qsTr("")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                    }
                }

                Text {
                    id: textPassword
                    color: "#ffffff"
                    text: qsTr("Mot de passe")
                    font.pixelSize: 20
                }

                Rectangle {
                    id: rectangleContainerPassword
                    width: parent.width
                    height: 30
                    color: "#ffffff"
                    border.width: 1
                    TextEdit {
                        id: textEditPassword
                        text: qsTr("")
                        anchors.fill: parent
                        font.pixelSize: 20
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                Rectangle {
                    id: rectangle1
                    width: parent.width
                    height: 5
                    color: "#00000000"
                }

                Rectangle {
                    id: buttonAuthentification
                    width: 181
                    height: 47
                    color: "#ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter

                    Text {
                        id: textButtonAuthentification
                        text: qsTr("S'authentifier")
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20

                        MouseArea {
                            id: mouseAreaButtonAuthentification
                            anchors.fill: parent

                            onClicked: ctrlGameBoard.authentifiate(textEditName.text, textEditPassword.text)
                        }
                    }
                }

            }
        }
    }

}
