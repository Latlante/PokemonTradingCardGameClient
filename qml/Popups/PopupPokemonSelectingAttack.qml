import QtQuick 2.0
import model 1.0

Item {
    id: popupPokemonSelectingAttack1

    property CardPokemon card: ctrlPopups.popupSelectingAttacks_Card

    signal clickedAttack(int index)
    //signal clickedAttack2
    //signal clickedAttack3

    width: 1000
    height: 500

    onCardChanged: {
        console.log("popupPokemonSelectingAttack1 cardChanged");

        if(card !== null)
        {
            imageCard.source = card.image;
            var heightPerAttack = 0.26 * imageCard.paintedHeight;
            rectangleAttack1.height = heightPerAttack / card.attacksCount();
            //rectangleRetreat.anchors.top = rectangleAttack1.anchors.bottom

            if(card.attacksCount() >= 2)
            {
                rectangleAttack2.visible = true;
                rectangleAttack2.height = heightPerAttack / card.attacksCount();
                //rectangleRetreat.anchors.top = rectangleAttack2.anchors.bottom
            }
            else
            {
                rectangleAttack2.visible = false;
                rectangleAttack2.height = 0;
            }

            if(card.attacksCount() >= 3)
            {
                rectangleAttack3.height = heightPerAttack / card.attacksCount();
                rectangleAttack3.visible = true;
                //rectangleRetreat.anchors.top = rectangleAttack3.anchors.bottom
            }
            else
            {
                rectangleAttack3.visible = false;
                rectangleAttack3.height = 0;
            }
        }
        else
        {
            console.log("popupPokemonSelectingAttack1 card is null");
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent

        color: "#AAAAAAAA"

        MouseArea {
            id: mouseAreaClosing
            anchors.fill: parent
            onClicked: ctrlPopups.selectionCardsFinished();
        }

        Image {
            id: imageCard
            anchors.fill: parent

            fillMode: Image.Pad
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            //source: card.image
            //source: "qrc:/Images/cartes/pokemon/1.png"

            Rectangle {
                id: rectangleAttack1
                width: imageCard.paintedWidth
                height: 0
                color: "transparent"
                anchors.verticalCenterOffset: (0.07 * imageCard.paintedHeight) + (0.5 * height)
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                border.color: "steelblue"
                border.width: 3

                MouseArea {
                    id: mouseAreaAttack1
                    anchors.fill: parent

                    onClicked: {
                        ctrlPopups.popupSelectingAttacks_IndexAttack = 0;
                        ctrlPopups.selectionCardsFinished();
                    }
                }
            }

            Rectangle {
                id: rectangleAttack2
                width: imageCard.paintedWidth
                height: 0
                color: "transparent"
                anchors.top: rectangleAttack1.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                border.color: "black"
                border.width: 3
                visible: false

                MouseArea {
                    id: mouseAreaAttack2
                    anchors.fill: parent

                    onClicked: {
                        ctrlPopups.popupSelectingAttacks_IndexAttack = 1;
                        ctrlPopups.selectionCardsFinished();
                    }
                }
            }

            Rectangle {
                id: rectangleAttack3
                width: imageCard.paintedWidth
                height: 0
                color: "transparent"
                anchors.top: rectangleAttack2.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                border.color: "red"
                border.width: 3
                visible: false

                MouseArea {
                    id: mouseAreaAttack3
                    anchors.fill: parent

                    onClicked: {
                        ctrlPopups.popupSelectingAttacks_IndexAttack = 2;
                        ctrlPopups.selectionCardsFinished();
                    }
                }
            }

            Rectangle {
                id: rectangleRetreat
                width: 0.33*imageCard.paintedWidth
                height: 0.1*imageCard.paintedHeight
                color: "transparent"
                anchors.top: rectangleAttack3.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: (imageCard.paintedWidth/2) - (width/2)
                border.color: "yellow"
                border.width: 3
                visible: ctrlPopups.popupSelectingAttacks_AuthorizeRetreat

                MouseArea {
                    id: mouseAreaRetreat
                    anchors.fill: parent

                    onClicked: {
                        ctrlPopups.popupSelectingAttacks_IndexAttack = 4;
                        ctrlPopups.selectionCardsFinished();
                    }
                }
            }
        }
    }
}
