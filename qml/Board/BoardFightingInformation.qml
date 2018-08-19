import QtQuick 2.0
import QtQuick.Controls 2.2
import model 1.0

Item {
    id: boardFightingInfo1
    width: 500
    height: 600

    property Player player

    Rectangle {
        id: rectangleFightingArea
        anchors.fill: parent
        color: "transparent"
        //border.color: "red"
        //border.width: 3

        ListView {
            id: listViewFightingArea
            anchors.fill: parent
            orientation: ListView.Horizontal
            interactive: false
            model: player.fight()
            delegate: Item {
                width: listViewFightingArea.width
                height: listViewFightingArea.height
                Rectangle {
                    id: delegateFightingAreaContainerInformation

                    property CardPokemon pokemon: model.card
                    property int heightLines: 50
                    property int pixelSize: heightLines - 10

                    onPokemonChanged: console.log("pokemon changed")

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    width: (3./5.) * parent.width
                    color: "transparent"

                    Image {
                        id: imageDelegateFightingAreaStatusPokemon
                        width: 50
                        height: width
                        anchors.top: parent.top
                        anchors.topMargin: 5
                        anchors.right: parent.right
                        source: {
                            switch(delegateFightingAreaContainerInformation.pokemon.status)
                            {
                            case CardPokemon.Status_Confused:   return "status/confused.png"
                            case CardPokemon.Status_Paralyzed:  return "status/paralyzed.png"
                            case CardPokemon.Status_Poisoned:   return "status/poison.png"
                            case CardPokemon.Status_Slept:      return "status/sleep.png"
                            default:                            return ""
                            }
                        }
                    }

                    Text {
                        id: textDelegateFightingAreaNamePokemon
                        height: delegateFightingAreaContainerInformation.heightLines
                        text: model.name
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        font.family: "Times New Roman"
                        font.pixelSize: delegateFightingAreaContainerInformation.pixelSize
                    }

                    Text {
                        id: textDelegateFightingAreaLifePokemon
                        height: delegateFightingAreaContainerInformation.heightLines
                        text: delegateFightingAreaContainerInformation.pokemon.lifeLeft + "/" + delegateFightingAreaContainerInformation.pokemon.lifeTotal + " PV"
                        anchors.top: textDelegateFightingAreaNamePokemon.bottom
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        font.pixelSize: delegateFightingAreaContainerInformation.pixelSize
                        font.family: "Times New Roman"
                    }

                    GridView {
                        id: gridViewDelegateFightingAreaEnergies
                        interactive: false
                        anchors.top: textDelegateFightingAreaLifePokemon.bottom
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.bottom: parent.bottom
                        model: delegateFightingAreaContainerInformation.pokemon.modelListOfEnergies()
                        cellWidth: 25
                        cellHeight: 25
                        delegate:
                            Item {
                                width: 25
                                height: 25

                                Image {
                                    id: imageModelIcon
                                    anchors.fill: parent
                                    source: model.icon
                                }
                        }
                    }
                }

                Rectangle {
                    id: delegateFightingAreaContainerImage
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    anchors.left: delegateFightingAreaContainerInformation.right
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    color: "transparent"

                    Image {
                        id: imageDelegateFightingAreaCardFighter
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: model.imageCard
                    }

                    MouseArea {
                        id: mouseAreadelegateFightingAreaCardFighter
                        anchors.fill: parent
                        propagateComposedEvents: true
                        onClicked: {
                            console.log("mouseAreaCardFighter clicked")

                            if(ctrlPopups.onePopupIsDisplayed === false)
                            {
                                if((delegateFightingAreaContainerInformation.pokemon !== null) &&
                                        (player.canPlay) &&
                                        (ctrlGameBoard.gameStatus === ConstantesQML.StepGameInProgress))
                                {

                                    ctrlGameBoard.attackRetreat(delegateFightingAreaContainerInformation.pokemon);
                                }

                                mouse.accepted = true
                            }
                            else
                                mouse.accepted = false

                        }

                        onPressAndHold: {
                            if(delegateFightingAreaContainerInformation.pokemon !== undefined)
                            {
                                popupCardDetailsComplete1.card = delegateFightingAreaContainerInformation.pokemon;
                                popupCardDetailsComplete1.visible = true;
                            }
                        }
                    }

                    DropArea {
                        id: dropAreaFightingArea
                        anchors.fill: parent
                        //onEntered: console.log("dropAreaFightingArea entered");
                        //onExited: console.log("dropAreaFightingArea exited");
                        onDropped: {
                            //Si ca vient du banc
                            if((drag.source.parent.player === boardFightingInfo1.player) &&
                                    (drag.source.objectName === "itemDelegatePacketBench"))
                            {
                                console.log("DropArea from bench");
                            }
                            //Ca vient de la main
                            else if((drag.source.parent.parent.player === boardFightingInfo1.player) &&
                                    (drag.source.objectName === "imageCardInHand"))
                            {
                                //boardFightingInfo1.player.moveCardFromHandToFight(drag.source.parent.dragItemIndex);
                                ctrlGameBoard.moveACard(4, drag.source.parent.dragItemIndex, 3, -1);
                            }


                        }
                    }
                }
            }
        }


        ButtonStyleGold {
            id: buttonReadyPreparation
            width: parent.width / 2
            height: 30
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            enabled: player.canPlay
            visible: ((player.initReady === false) &&
                      (ctrlGameBoard.gameStatus === ConstantesQML.StepPreparation))

            text: "Prêt"

            onClicked: {
                console.log("buttonReadyPreparation clicked")
                //player.checkIfInitReady()
                ctrlGameBoard.initReady()
            }
        }

        ButtonStyleGold {
            id: buttonFinishYourTurn
            width: parent.width / 2
            height: 30
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            visible: (player.canPlay) &&
                     (ctrlGameBoard.gameStatus === ConstantesQML.StepGameInProgress)

            text: "Terminer votre tour"

            onClicked: {
                console.log("buttonFinishYourTurn clicked")
                ctrlGameBoard.skipTheTurn()
            }
        }

    }
}
