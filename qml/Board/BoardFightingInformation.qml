import QtQuick 2.0
import QtQuick.Controls 2.2
import model 1.0

Item {
    id: boardFightingInfo1
    width: 500
    height: 600

    property Player player
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
                                (player.canPlay) &&
                                (ctrlGameBoard.gameStatus === ConstantesQML.StepGameInProgress))
                        {

                            ctrlGameBoard.attackRetreat(cardPok);
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

            ButtonStyleGold {
                id: buttonReadyPreparation
                height: 30
                anchors.left: parent.left
                anchors.right: parent.right
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
                height: 30
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                visible: (cardPok !== undefined) && (player.canPlay) &&
                         (ctrlGameBoard.gameStatus === ConstantesQML.StepGameInProgress)

                text: "Terminer votre tour"

                onClicked: {
                    console.log("buttonFinishYourTurn clicked")
                    ctrlGameBoard.skipTheTurn()
                }
            }

        }
    }
}
