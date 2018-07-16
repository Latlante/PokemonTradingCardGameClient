import QtQuick 2.0
import QtQuick.Controls 2.2
import model 1.0

Item {
    id: boardFightingInfo1
    width: 500
    height: 600

    property Player player
    property Player currentPlayer
    property CardPokemon cardPok: player.fight().pokemonFighter
    property string pokemonName: cardPok.name
    property string pokemonImage: cardPok.image
    property int pokemonLifeTotal: cardPok.lifeTotal
    property int pokemonLifeLeft: /*cardPok === null ? 0 : */cardPok.lifeLeft
    property int pokemonStatus: /*cardPok === null ? 0 : */cardPok.status

    onPlayerChanged: {
        console.log("boardFightingInfo1 onPlayerChanged");
    }

    onPokemonNameChanged: {
        console.log("boardFightingInfo1 onPokemonNameChanged");
        textNamePokemon.text = pokemonName;
    }

    onPokemonImageChanged: {
        console.log("boardFightingInfo1 onPokemonImageChanged");
        pokemonFighting.source = pokemonImage;
    }

    onPokemonLifeTotalChanged: {
        console.log("boardFightingInfo1 onPokemonLifeTotalChanged");
        textLifePokemon.text = pokemonLifeLeft + "/" + pokemonLifeTotal + " PV"
    }

    onPokemonLifeLeftChanged: {
        console.log("boardFightingInfo1 onPokemonLifeLeftChanged");
        textLifePokemon.text = pokemonLifeLeft + "/" + pokemonLifeTotal + " PV"
    }

    onPokemonStatusChanged: {
        console.log("boardFightingInfo1 onPokemonStatusChanged = " + pokemonStatus);

        switch(pokemonStatus)
        {
        case CardPokemon.Status_Confused:
            imageStatusPokemon.source = "status/confused.png";
            break;
        case CardPokemon.Status_Paralyzed:
            imageStatusPokemon.source = "status/paralyzed.png";
            break;
        case CardPokemon.Status_Poisoned:
            imageStatusPokemon.source = "status/poison.png";
            break;
        case CardPokemon.Status_Slept:
            imageStatusPokemon.source = "status/sleep.png";
            break;
        default:
            imageStatusPokemon.source = "";
            break;
        }
    }

    onCardPokChanged: {
        //console.log("boardFightingInfo1 onCardPokChanged");
        if(cardPok !== null)
        {
            //console.log("boardFightingInfo1 onPlayerChanged: player not null");
            gridViewEnergies.model = cardPok.modelListOfEnergies();
        }
        else
        {
            //console.log("boardFightingInfo1 onPlayerChanged: player is null");
            pokemonFighting.source = "back.png"
        }
    }



    Rectangle {
        id: rectangleFightingArea
        anchors.fill: parent
        color: "#00000000"
        //border.color: "red"
        //border.width: 3

        Image {
            id: pokemonFighting
            x: 295
            width: (2./5.) * parent.width
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            fillMode: Image.PreserveAspectFit
            source: "back.png"

            MouseArea {
                id: mouseAreaCardFighter
                anchors.fill: parent
                propagateComposedEvents: true
                onClicked: {
                    console.log("mouseAreaCardFighter clicked")

                    if(ctrlPopups.onePopupIsDisplayed === false)
                    {
                        if((cardPok !== undefined) &&
                                (player === currentPlayer) &&
                                (ctrlGameBoard.gameStatus === ConstantesQML.StepGameInProgress))
                        {

                            ctrlGameBoard.actionAttack(cardPok);
                        }

                        mouse.accepted = true
                    }
                    else
                        mouse.accepted = false

                }

                onPressAndHold: {
                    if(cardPok !== undefined)
                    {
                        popupCardDetailsComplete1.card = cardPok;
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
                    //console.log("DropArea in fightingArea");
                    //console.log("drag.source: " + drag.source.parent.objectName);
                    //Si ca vient du banc
                    if((drag.source.parent.player === boardFightingInfo1.player) &&
                            (drag.source.objectName === "itemDelegatePacketBench"))
                    {
                        console.log("DropArea from bench");
                        //boardFightingInfo1.player.moveCardFromBenchToFight(drag.source.parent.listViewPacketBench.dragItemIndex);
                        //drag.source.parent.listViewPacketBench.dragItemIndex = -1;
                    }
                    //Ca vient de la main
                    else if((drag.source.parent.parent.player === boardFightingInfo1.player) &&
                            (drag.source.objectName === "imageCardInHand"))
                    {
                        //console.log("DropArea from hand, " + drag.source.parent.objectName);
                        boardFightingInfo1.player.moveCardFromHandToFight(drag.source.parent.dragItemIndex);
                        //drag.source.parent.listViewPacketHand.dragItemIndex = -1;
                    }

                    /*if((boardPlayer1.listViewPacketBench.dragItemIndex !== -1) &&
                            (boardPlayer1.player === boardFightingInfo1.player))
                    {
                        console.log("DropArea from bench");
                        player.moveCardFromBenchToFight()
                    }
                    if((boardPlayer1.listViewPacketBench.dragItemIndex !== -1) &&
                            (boardPlayer1.player === boardFightingInfo1.player))
                    {
                        listView.model.append(listView3.model.get(listView3.dragItemIndex))
                        listView3.model.remove(listView3.dragItemIndex)
                        listView3.dragItemIndex = -1;
                    }*/
                }
            }
        }

        Rectangle {
            id: rectangleContainerInformation

            property int heightLines: 50
            property int pixelSize: heightLines - 10

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: pokemonFighting.left
            anchors.bottom: parent.bottom
            color: "transparent"
            //border.color: "black"
            //border.width: 3

            Image {
                id: imageStatusPokemon
                width: 50
                height: width
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.right: parent.right
            }

            Text {
                id: textNamePokemon
                height: rectangleContainerInformation.heightLines
                //text: qsTr("Pokemon")
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                font.family: "Times New Roman"
                font.pixelSize: rectangleContainerInformation.pixelSize
            }

            Text {
                id: textLifePokemon
                height: rectangleContainerInformation.heightLines
                //text: cardPok.lifeLeft + "/" + cardPok.lifeTotal() + " PV"
                anchors.top: textNamePokemon.bottom
                anchors.topMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.right: parent.right
                anchors.rightMargin: 0
                font.pixelSize: rectangleContainerInformation.pixelSize
                font.family: "Times New Roman"
            }

            GridView {
                id: gridViewEnergies
                interactive: false
                anchors.top: textLifePokemon.bottom
                anchors.topMargin: 0
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.bottom: buttonReadyPreparation.top
                //model: cardPok.modelListOfEnergies()
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

            /*Button {
                id: buttonAttack
                height: 30
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                //enabled:
                visible: player === currentPlayer

                text: "Attaquer"

                onClicked: ctrlGameBoard.onClicked_ButtonAttack(0)
            }*/

            ButtonStyleGold {
                id: buttonReadyPreparation
                height: 30
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                visible: ((player.initReady === false) &&
                          (ctrlGameBoard.gameStatus === ConstantesQML.StepPreparation))

                text: "Prêt"

                onClicked: {
                    console.log("buttonReadyPreparation clicked")
                    player.checkIfInitReady()
                    ctrlGameBoard.onClicked_ButtonReadyPreparation()
                }
            }

            ButtonStyleGold {
                id: buttonFinishYourTurn
                height: 30
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                visible: (cardPok !== undefined) && (player === currentPlayer) &&
                         (ctrlGameBoard.gameStatus === ConstantesQML.StepGameInProgress)

                text: "Terminer votre tour"

                onClicked: {
                    console.log("buttonFinishYourTurn clicked")
                    ctrlGameBoard.actionFinishYourTurn()
                }
            }

        }
    }
}
