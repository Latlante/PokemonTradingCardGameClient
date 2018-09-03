import QtQuick 2.0
import model 1.0

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
                    switch(ctrlAnimation.movingCardLocationStart)
                    {
                    case CtrlAnimation.Location_Bench:
                        movingCard1.x = boardPlayer1.listViewPacketBench.x
                        movingCard1.y = boardPlayer1.listViewPacketBench.y
                        break;
                    case CtrlAnimation.Location_Deck:
                        movingCard1.x = boardPlayer1.textDeck.x
                        movingCard1.y = boardPlayer1.textDeck.y
                        break;
                    case CtrlAnimation.Location_Fight:
                        movingCard1.x = boardPlayer1.boardFightingArea1.boardFightingInformation_P1.x
                        movingCard1.y = boardPlayer1.boardFightingArea1.boardFightingInformation_P1.y
                        break;
                    case CtrlAnimation.Location_Hand:
                        movingCard1.x = boardPlayer1.listViewPacketHand.x
                        movingCard1.y = boardPlayer1.listViewPacketHand.y
                        break;
                    case CtrlAnimation.Location_Reward:
                        movingCard1.x = boardPlayer1.textRewards.x
                        movingCard1.y = boardPlayer1.textRewards.y
                        break;
                    case CtrlAnimation.Location_Trash:
                        movingCard1.x = boardPlayer1.textTrash.x
                        movingCard1.y = boardPlayer1.textTrash.y
                        break;
                    }

                    switch(ctrlAnimation.movingCardLocationEnd)
                    {
                    case CtrlAnimation.Location_Bench:
                        movingCard1.positionXEnd = boardPlayer1.listViewPacketBench.x
                        movingCard1.positionYEnd = boardPlayer1.listViewPacketBench.y
                        break;
                    case CtrlAnimation.Location_Deck:
                        movingCard1.positionXEnd = boardPlayer1.textDeck.x
                        movingCard1.positionYEnd = boardPlayer1.textDeck.y
                        break;
                    case CtrlAnimation.Location_Fight:
                        movingCard1.positionXEnd = boardPlayer1.boardFightingArea1.boardFightingInformation_P1.x
                        movingCard1.positionYEnd = boardPlayer1.boardFightingArea1.boardFightingInformation_P1.y
                        break;
                    case CtrlAnimation.Location_Hand:
                        movingCard1.positionXEnd = boardPlayer1.listViewPacketHand.x
                        movingCard1.positionYEnd = boardPlayer1.listViewPacketHand.y
                        break;
                    case CtrlAnimation.Location_Reward:
                        movingCard1.positionXEnd = boardPlayer1.textRewards.x
                        movingCard1.positionYEnd = boardPlayer1.textRewards.y
                        break;
                    case CtrlAnimation.Location_Trash:
                        movingCard1.positionXEnd = boardPlayer1.textTrash.x
                        movingCard1.positionYEnd = boardPlayer1.textTrash.y
                        break;
                    }

                    startAnimation = animationStatus
                }

            }

            onAnimationRunningChanged: {
                if(running == false)
                    ctrlAnimation.animationMovingCardFinished()
            }
        }
    }
}

