import QtQuick 2.0
import QtQuick.Controls 2.2
import model 1.0

Item {
    id: selectCards1
    width: 500
    height: 900

    property int colCount: 4
    property int space: 4   //PAS DE NOMBRE IMPAIRE
    property bool lastPlayer: viewCards.model.lastPlayer

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#2b95ff"

        Image {
            id: imageSave
            width: namePlayer.height
            height: namePlayer.height
            anchors.top: parent.top
            anchors.right: parent.right
            source: "textures/save.png"

            MouseArea {
                id: mouseAreaSave
                anchors.fill: parent
                onClicked: ctrlSelectingCards.savePacket()
            }
        }

        Image {
            id: imageLoad
            width: namePlayer.height
            height: namePlayer.height
            anchors.top: parent.top
            anchors.right: imageSave.left
            anchors.rightMargin: 10
            source: "textures/open.png"

            MouseArea {
                id: mouseAreaLoad
                anchors.fill: parent
                onClicked: ctrlSelectingCards.loadPacket()
            }
        }

        Text {
            id: namePlayer
            height: 40
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 35
            text: viewCards.model.name + " (" + viewCards.model.countTotalQuantity + " / " + viewCards.model.maxCards() + ")"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        GridView {
            id: viewCards
            anchors.top: namePlayer.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: buttonRandom.top
            anchors.bottomMargin: 10
            //cellWidth: (viewCards.width-selectCards1.space)/selectCards1.colCount
            //cellHeight: cellWidth*2
            cellWidth: 160
            cellHeight: 270
            clip: true

            model: ctrlSelectingCards.model()
            delegate: Item {
                id: itemCard
                //anchors.fill: parent
                width: 150
                height: 260

                Image {
                    id: imageCard
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: barSelectQuantityCards.top
                    fillMode: Image.PreserveAspectFit
                    source: model.imageCard

                    MouseArea {
                        id: mouseAreaCard
                        anchors.fill: parent
                        onClicked: {
                            popupCardDetailsBasic1.card = model.card;
                            popupCardDetailsBasic1.visible = true;
                        }
                    }
                }

                Rectangle {
                    id: barSelectQuantityCards
                    height: 40
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    Button {
                        id: buttonMinus
                        width: parent.width / 4
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        text: "-"

                        onClicked: ctrlSelectingCards.removeACard(index)
                    }

                    Rectangle {
                        id: backgroundTextQuantity
                        anchors.top: parent.top
                        anchors.left: buttonMinus.right
                        anchors.right: buttonPlus.left
                        anchors.bottom: parent.bottom
                        color: "white"

                        Text {
                            id: textInputQuantity
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: model.quantity
                        }
                    }

                    Button {
                        id: buttonPlus
                        width: parent.width / 4
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        text: "+"

                        onClicked: ctrlSelectingCards.addANewCard(index)
                    }
                }
            }

        }

        ButtonStyleGold {
            id: buttonRandom
            height: 40
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: lastPlayer === true ? buttonOk.top : buttonNext.top
            anchors.bottomMargin: 10
            text: "Générer deck aléatoire"

            onClicked: ctrlSelectingCards.fillARandomList()
        }

        ButtonStyleGold {
            id: buttonNext
            height: 40
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            visible: lastPlayer === false
            text: "Joueur suivant"
            enabled: viewCards.model.countTotalQuantity === viewCards.model.maxCards()

            onClicked: ctrlSelectingCards.onClickedListFinished()
        }

        ButtonStyleGold {
            id: buttonOk
            height: 40
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            visible: lastPlayer === true
            text: "OK"
            enabled: viewCards.model.countTotalQuantity === viewCards.model.maxCards()

            onClicked: ctrlGameBoard.onClicked_ButtonOk_SelectCards()
        }
    }

    PopupCardDetailsBasic {
        id: popupCardDetailsBasic1
        anchors.fill: parent
        visible: false
    }
}
