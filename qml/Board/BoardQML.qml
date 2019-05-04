import QtQuick 2.0
import model 1.0
import "qml/BasicComponents"

Item {
    id: board1
    visible: true
    width: 1200
    height: 1920

    property Player player1: ctrlGameBoard.playerYou()
    property Player player2: ctrlGameBoard.playerOpponent()

    /*Image {
        id: background
        fillMode: Image.Tile
        anchors.fill: parent
        source: "qrc:/textures/tapis.jpg"*/

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#2b95ff"

        //ZONE DE COMBAT CENTRALE
        BoardFightingArea {
            id: boardFightingArea1
            height: 0.25 * parent.height
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            play1: board1.player1
            play2: board1.player2
        }

        //ZONE DE JOUEUR 2
        BoardPLayerAreaQML {
            id: boardPlayer2
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: boardFightingArea1.top
            rotation: 180

            player: player2
        }

        //ZONE DE JOUEUR 1
        BoardPLayerAreaQML {
            id: boardPlayer1
            anchors.top: boardFightingArea1.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            player: player1
        }

        //POP-UPS
        PopupCardDetailsBasic {
            id: popupCardDetailsBasic1
            anchors.fill: parent
            visible: false
        }

        PopupCardDetailsComplete {
            id: popupCardDetailsComplete1
            anchors.fill: parent
            visible: false
        }

        PopupPokemonSelectingAttack {
            id: popupPokemonSelectingAttack1
            anchors.fill: parent
            visible: ctrlPopups.popupSelectingAttacks_Visible
        }

        PopupSelectCardsInPacket {
            id: popupSelectCardInPacket1
            anchors.fill: parent
            visible: ctrlPopups.selectCardInPacketVisible
        }

        PopupSelectHiddenCard {
            id: popupSelectHiddenCard1
            anchors.fill: parent
            visible: ctrlPopups.selectHiddenCardVisible
        }

        PopupSelectEnergiesInPokemon {
            id: popupSelectEnergiesInPokemon1
            anchors.fill: parent
            visible: ctrlPopups.selectEnergiesInPokemonVisible
        }

        PopupMessage {
            id: popupMessage1
            anchors.fill: parent
            visible: ctrlPopups.messageVisible
        }

        PopupCoinHeadOrTail {
            id: popupCoinHeadOrTail1
            anchors.fill: parent
            visible: ctrlPopups.headOrTailVisible
        }  

        MovingCard {
            id: movingCard1

            property bool animationStatus: ctrlAnimation.movingCardStarted

            sourceImage: "back.png"
            visible: false

            onAnimationStatusChanged: {
                if(animationStatus == true)
                {
                    console.log("ANIMATION MOVED STARTED");

                    var boardPlayerSelected = undefined;
                    if(ctrlAnimation.movingCardPlayer == player1)
                    {
                        boardPlayerSelected = boardPlayer1;
                        console.log("player1 selected at position (" + boardPlayerSelected.x + "x" + boardPlayerSelected.y + ")");
                    }
                    else if(ctrlAnimation.movingCardPlayer == player2)
                    {
                        console.log("player2 selected");
                        boardPlayerSelected = boardPlayer2;
                    }
                    else
                        console.error("Animation impossible, no player selected");

                    if(boardPlayerSelected != undefined)
                    {
                        switch(ctrlAnimation.movingCardLocationStart)
                        {
                        case CtrlAnimation.Location_Bench:
                            console.log("QML: card moved from listViewPacketBench (" + boardPlayerSelected.posBenchX + "x" + boardPlayerSelected.posBenchY + ")")
                            movingCard1.x = boardPlayerSelected.posBenchX + boardPlayerSelected.x
                            movingCard1.y = boardPlayerSelected.posBenchY + boardPlayerSelected.y
                            break;
                        case CtrlAnimation.Location_Deck:
                            console.log("QML: card moved from textDeck (" + boardPlayerSelected.posDeckX + "x" + boardPlayerSelected.posDeckY + ")")
                            movingCard1.x = boardPlayerSelected.posDeckX + boardPlayerSelected.x
                            movingCard1.y = boardPlayerSelected.posDeckY + boardPlayerSelected.y
                            break;
                        case CtrlAnimation.Location_Fight:
                            console.log("QML: card moved from boardFightingInformation_P1 (" + boardFightingArea1.posFightX + "x" + boardFightingArea1.posFightY + ")")
                            movingCard1.x = boardFightingArea1.posFightX
                            movingCard1.y = boardFightingArea1.posFightY
                            break;
                        case CtrlAnimation.Location_Hand:
                            console.log("QML: card moved from listViewPacketHand (" + boardPlayerSelected.posHandX + "x" + boardPlayerSelected.posHandY + ")")
                            movingCard1.x = boardPlayerSelected.posHandX + boardPlayerSelected.x
                            movingCard1.y = boardPlayerSelected.posHandY + boardPlayerSelected.y
                            break;
                        case CtrlAnimation.Location_Reward:
                            console.log("QML: card moved from textRewards (" + boardPlayerSelected.posRewardY + "x" + boardPlayerSelected.posRewardY + ")")
                            movingCard1.x = boardPlayerSelected.posRewardX + boardPlayerSelected.x
                            movingCard1.y = boardPlayerSelected.posRewardY + boardPlayerSelected.y
                            break;
                        case CtrlAnimation.Location_Trash:
                            console.log("QML: card moved from textTrash (" + boardPlayerSelected.posTrashX + "x" + boardPlayerSelected.posTrashY + ")")
                            movingCard1.x = boardPlayerSelected.posTrashX + boardPlayerSelected.x
                            movingCard1.y = boardPlayerSelected.posTrashY + boardPlayerSelected.y
                            break;
                        }

                        switch(ctrlAnimation.movingCardLocationEnd)
                        {
                        case CtrlAnimation.Location_Bench:
                            console.log("to listViewPacketBench (" + boardPlayerSelected.posBenchX + "x" + boardPlayerSelected.posBenchY + ")")
                            movingCard1.positionXEnd = boardPlayerSelected.posBenchX + boardPlayerSelected.x
                            movingCard1.positionYEnd = boardPlayerSelected.posBenchY + boardPlayerSelected.y
                            break;
                        case CtrlAnimation.Location_Deck:
                            console.log("to textDeck (" + boardPlayerSelected.posDeckX + "x" + boardPlayerSelected.posDeckY + ")")
                            movingCard1.positionXEnd = boardPlayerSelected.posDeckX + boardPlayerSelected.x
                            movingCard1.positionYEnd = boardPlayerSelected.posDeckY + boardPlayerSelected.y
                            break;
                        case CtrlAnimation.Location_Fight:
                            console.log("to boardFightingInformation_P1 (" + boardFightingArea1.posFightX + "x" + boardFightingArea1.posFightY + ")")
                            movingCard1.positionXEnd = boardFightingArea1.posFightX
                            movingCard1.positionYEnd = boardFightingArea1.posFightY
                            break;
                        case CtrlAnimation.Location_Hand:
                            console.log("to listViewPacketHand (" + boardPlayerSelected.posHandX + "x" + boardPlayerSelected.posHandY + ")")
                            movingCard1.positionXEnd = boardPlayerSelected.posHandX + boardPlayerSelected.x
                            movingCard1.positionYEnd = boardPlayerSelected.posHandY + boardPlayerSelected.y
                            break;
                        case CtrlAnimation.Location_Reward:
                            console.log("to textRewards (" + boardPlayerSelected.posRewardY + "x" + boardPlayerSelected.posRewardY + ")")
                            movingCard1.positionXEnd = boardPlayerSelected.posRewardX + boardPlayerSelected.x
                            movingCard1.positionYEnd = boardPlayerSelected.posRewardY + boardPlayerSelected.y
                            break;
                        case CtrlAnimation.Location_Trash:
                            console.log("to textTrash (" + boardPlayerSelected.posTrashX + "x" + boardPlayerSelected.posTrashY + ")")
                            movingCard1.positionXEnd = boardPlayerSelected.posTrashX + boardPlayerSelected.x
                            movingCard1.positionYEnd = boardPlayerSelected.posTrashY + boardPlayerSelected.y
                            break;
                        }

                        startAnimation = animationStatus
                    }
                }
            }

            onAnimationRunningChanged: {
                if(running == false)
                    ctrlAnimation.animationMovingCardFinished()
            }
        }
    }
}

