import QtQuick 2.0
import QtQuick.Controls 2.2
import model 1.0

Item {
    id: popupSelectHiddenCard1
    width: 500
    height: 900

    property int colCount: 4
    property int space: 4   //PAS DE NOMBRE IMPAIRE

    property int numberOfCardsSelected: viewCards.model.numberOfCardsSelected

    onNumberOfCardsSelectedChanged: {
        title.text = "Sélection (" + viewCards.model.numberOfCardsSelected + "/" + viewCards.model.numberOfCardsToSelect() + ")";
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#AAAAAAAA"

        MouseArea {
            anchors.fill: parent
            onClicked: console.log("popupSelectHiddenCard1 clicked back")
        }

        Text {
            id: title
            height: 40
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 35
            text: "Sélection (" + viewCards.model.numberOfCardsSelected + "/" + viewCards.model.numberOfCardsToSelect() + ")"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        GridView {
            id: viewCards
            anchors.top: title.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: buttonOk.top
            anchors.bottomMargin: 10
            cellWidth: 160
            cellHeight: 270
            clip: true

            model: ctrlPopups.modelSelectCardInPacket()
            delegate: Item {
                id: itemCard
                //anchors.fill: parent
                width: 150
                height: 240

                Rectangle {
                    id: rectangleContainerImageCard
                    anchors.fill: parent

                    color: model.selected ? "#2b95ff" : "transparent"

                    HiddenCard {
                        anchors.fill: parent
                        anchors.margins: 3
                        imagePokemon: model.imageCard
                        flipped: model.flip

                        MouseArea {
                            id: mouseAreaCard
                            anchors.fill: parent
                            onClicked: {
                                model.selected = !model.selected
                            }
                        }
                    }

                }
            }
        }

        Button {
            id: buttonOk
            height: 40
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            enabled: viewCards.model.isMaximumCardsSelected === true
            text: "OK"

            onClicked: {
                //On bloque l'affiche pour que l'utilisateur ne clique pas partout
                popupSelectHiddenCard1.enabled = false

                //On lance l'animation pour retourner la carte
                viewCards.model.flipIfSelected()

                //On lance le timer avant que la page se ferme
                timerBeforeClosing.start()
            }
        }

        Timer {
            id: timerBeforeClosing
            interval: 3000;
            running: false;
            repeat: false
            onTriggered: {
                ctrlPopups.selectionCardsFinished()
                popupSelectHiddenCard1.enabled = true
            }
        }
    }
}
