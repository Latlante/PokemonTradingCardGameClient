import QtQuick 2.0
import QtQuick.Controls 2.2
import model 1.0

Item {
    id: selectCards1
    width: 500
    height: 900

    property int colCount: 4
    property int space: 4   //PAS DE NOMBRE IMPAIRE
    property string nameOfThePlayer: ctrlSelectingCards.model().name
    property int countTotalQuantity: ctrlSelectingCards.model().countTotalQuantity
    property int maxCards: ctrlSelectingCards.model().maxCards()
    property bool stepInProgress: ctrlAnimation.stepInProgress

    //onCountTotalQuantityChanged: console.log("onCountTotalQuantityChanged:" + countTotalQuantity)

    onStepInProgressChanged: {
        if(stepInProgress == false)
        {
            background.enabled = true
            loading_P1.visible = false
        }
        else if(stepInProgress == true)
        {
            background.enabled = false
            loading_P1.visible = true
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#2b95ff"

        ComboBox {
            id: comboBoxFilterSelectingCards
            width: 200
            height: namePlayer.height
            anchors.top: parent.top
            anchors.left: parent.left
            model: ctrlSelectingCards.modelFilters().listInfo
            onCurrentIndexChanged: ctrlSelectingCards.setFilterProxy(ctrlSelectingCards.modelFilters().filter(comboBoxFilterSelectingCards.currentIndex))
        }

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
                onClicked: {
                    popupCardLineEdit1.visible = true;
                    //ctrlSelectingCards.savePacket()
                }
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
                onClicked: {
                    ctrlGameBoard.displayPageSelection()
                    //ctrlSelectingCards.loadPacket("")
                }
            }
        }


        Text {
            id: namePlayer
            height: 40
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 35
            text: nameOfThePlayer + " (" + countTotalQuantity + " / " + maxCards + ")"
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

            model: ctrlSelectingCards.proxy()
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

                        onClicked: model.quantity = model.quantity - 1
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

                        onClicked: model.quantity = model.quantity + 1
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
            anchors.bottom: buttonOk.top
            anchors.bottomMargin: 10
            text: "Générer deck aléatoire"

            onClicked: ctrlSelectingCards.fillARandomList()
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
            text: "OK"
            enabled: (selectCards1.countTotalQuantity === selectCards1.maxCards) && (stepInProgress === false)

            onClicked: ctrlGameBoard.sendCardsSelected()
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

    PopupCardDetailsBasic {
        id: popupCardDetailsBasic1
        anchors.fill: parent
        visible: false
    }

    PopupLineEdit {
        id: popupCardLineEdit1
        anchors.fill: parent
        visible: false
        onClicked: ctrlSelectingCards.savePacket(result);
    }

    /*Popup {
        id: popupLineEdit
        width: 200
        height: 300
        modal: true
        focus: true
        visible: false
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Column {
            anchors.fill: parent
            Text {
                text: "Nom de la partie"
            }

            Rectangle {
                id: rectangleContainerLineEdit
                height: 25
                color: "#ffffff"
                border.width: 1

                TextInput {
                    id: textInputLineEdit
                    text: qsTr("Partie n1")
                    horizontalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                    font.pixelSize: 18
                }
            }

            ButtonStyleGold {
                id: boutonOkLineEdit

                //width: parent.width
                height: 40
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                text: qsTr("Ok")

                onClicked: console.log("clicked ok")
            }
        }
    }*/
}
