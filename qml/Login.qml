import QtQuick 2.0
import model 1.0
import "qml/BasicComponents"

Item {
    height: 600

    property bool stepInProgress: ctrlAnimation.stepInProgress

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
            x: 0
            y: 0
            width: parent.width
            height: 0.3 * parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectFit
            source: "Images/logo.png"
        }

        Rectangle {
            id: rectangleContainerAuthentification
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

                            onClicked: {
                                console.log("bouton Authentification cliqué")
                                if((textEditName.text != "") && (textEditPassword.text != ""))
                                    ctrlGameBoard.authentificate(textEditName.text, textEditPassword.text)
                                else
                                    console.log("Error: login and/or password is empty");
                            }
                        }
                    }
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

        //ANIMATION OPENING
        function initPageAnimationOpening()
        {
            imageLogo.y = (rectangleBackground.height / 2) - (imageLogo.height / 2)
            rectangleContainerAuthentification.opacity = 0

            animationOpening.start()
        }

        SequentialAnimation {
            id: animationOpening

            PauseAnimation {
                duration: 1000
            }
            PropertyAnimation {
                target: imageLogo
                properties: "scale"
                //from: 0
                to: 1.2
                duration: 500
            }
            PropertyAnimation {
                target: imageLogo
                properties: "scale"
                //from: 0
                to: 1
                duration: 500
            }
            PropertyAnimation {
                target: imageLogo
                properties: "y"
                //from: 0
                to: 0
                duration: 1000
            }
            PropertyAnimation {
                target: rectangleContainerAuthentification
                properties: "opacity"
                //from: 0
                to: 1
                duration: 1000
            }
        }

        Component.onCompleted: initPageAnimationOpening()
    }



}
