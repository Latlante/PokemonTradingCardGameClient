import QtQuick 2.0
import QtQuick.Controls 2.2
import model 1.0

Item {

    Rectangle {
        id: rectangle
        color: "#2b95ff"
        anchors.fill: parent

        Image {
            id: imageLogo
            height: parent.height / 3
            anchors.top: parent.top
            //anchors.topMargin: 10
            anchors.left: parent.left
            //anchors.leftMargin: 10
            anchors.right: parent.right
            //anchors.rightMargin: 10
            anchors.margins: 20
            fillMode: Image.PreserveAspectFit

            source: "Images/logo.png"
        }

        Rectangle {
            id: rectangleNumberOfPlayers
            height: 40
            anchors.top: imageLogo.bottom
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0

            Row {
                anchors.fill: parent
                spacing: 20

                Text {
                    id: textNumberOfPlayers
                    color: "#000000"
                    width: 200
                    height: 40
                    text: qsTr("Nombre de joueurs:")
                    /*anchors.top: imageLogo.bottom
                    anchors.topMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0*/
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 20
                }

                Button {
                    id: buttonTwoPlayers
                    width: 100
                    height: 40
                    text: "2 joueurs"
                    onClicked: listView.model.setNumberOfPlayers(2)
                }

                Button {
                    id: buttonFourPlayers
                    width: 100
                    height: 40
                    text: "4 joueurs (soon)"
                    enabled: false
                    onClicked: listView.model.setNumberOfPlayers(4)
                }
            }
        }



        Rectangle {
            id: rectangleContainerListView
            width: 200
            color: "transparent"
            anchors.horizontalCenter: parent.horizontalCenter
            //border.width: 3
            anchors.top: rectangleNumberOfPlayers.bottom
            anchors.topMargin: 10
            anchors.bottom: buttonValidate.top
            anchors.bottomMargin: 10

            ListView {
                id: listView
                anchors.fill: parent
                spacing: 50
                interactive: false
                model: ctrlGameBoard.newListPlayers()
                delegate: Rectangle {
                    /*anchors.left: parent.left
                    anchors.right: parent.right*/
                    width: parent.width
                    height: 40
                    color: "#ffffff"
                    border.color: "#000000"
                    border.width: 2

                    TextInput {
                        anchors.fill: parent
                        font.pixelSize: 35
                        text: model.name
                    }
                }
            }
        }

        /*Button {
            id: buttonAddNewPlyer
            width: (parent.width / 2) - 20
            text: qsTr("Ajouter un nouveau joueur")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: buttonValidate.top
            anchors.bottomMargin: 10

            onClicked: listView.model.addNewPlayer()
        }

        Button {
            id: buttonRemoveOnePlayer
            y: 400
            text: qsTr("Supprimer un joueur")
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: buttonAddNewPlyer.right
            anchors.leftMargin: 10
            anchors.bottom: buttonValidate.top
            anchors.bottomMargin: 10

            onClicked: listView.model.removeOnePlayer()
        }*/

        ButtonStyleGold {
            id: buttonValidate
            height: 40
            text: qsTr("Valider")
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10

            onClicked: ctrlGameBoard.onClicked_ButtonOk_SelectPlayers(listView.model.listOfPlayers)
        }

    }
}
