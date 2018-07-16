import QtQuick 2.0
import model 1.0

Item {
    id: boardPlayerArea1
    objectName: "boardPlayerArea1"
    width: 1000
    height: 600

    property Player player

    enabled: player.canPlay


    // |-----------------|
    // |***** Infos *****|
    // |-----------------|
    // |      Bench      |
    // |-----------------|
    // |       Hand      |
    // |-----------------|
    Rectangle {
        id: rectangleInfosPackets
        height: 50
        color: "#00000000"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.leftMargin: 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 0        

        Text {
            id: textNamePlayer
            verticalAlignment: Text.AlignVCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            font.pixelSize: 30
            text: player.name()
        }

        Text {
            id: textDeckCst
            width: 80
            height: 45
            text: qsTr("Deck:")
            anchors.right: textDeck.left
            anchors.rightMargin: 0
            verticalAlignment: Text.AlignVCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            fontSizeMode: Text.HorizontalFit
            renderType: Text.NativeRendering
            font.pixelSize: 30
        }

        Text {
            id: textDeck
            width: 50
            text: player.deck().countCard
            anchors.right: textTrashCst.left
            anchors.rightMargin: 0
            verticalAlignment: Text.AlignVCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            font.pixelSize: 30
            fontSizeMode: Text.HorizontalFit
            renderType: Text.NativeRendering
        }

        Text {
            id: textTrashCst
            width: 92
            height: 45
            text: qsTr("Trash:")
            anchors.right: textTrash.left
            anchors.rightMargin: 0
            verticalAlignment: Text.AlignVCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            font.pixelSize: 30
            fontSizeMode: Text.HorizontalFit
            renderType: Text.NativeRendering
        }

        Text {
            id: textTrash
            width: 50
            text: player.trash().countCard
            anchors.right: textRewardsCst.left
            anchors.rightMargin: 0
            verticalAlignment: Text.AlignVCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            font.pixelSize: 30
            fontSizeMode: Text.HorizontalFit
            renderType: Text.NativeRendering
        }

        Text {
            id: textRewardsCst
            width: 144
            height: 45
            text: qsTr("Rewards:")
            anchors.right: textRewards.left
            anchors.rightMargin: 0
            verticalAlignment: Text.AlignVCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            font.pixelSize: 30
            fontSizeMode: Text.HorizontalFit
            renderType: Text.NativeRendering
        }

        Text {
            id: textRewards
            width: 50
            text: player.rewards().countCard
            anchors.right: parent.right
            anchors.rightMargin: 0
            verticalAlignment: Text.AlignVCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            font.pixelSize: 30
            fontSizeMode: Text.HorizontalFit
            renderType: Text.NativeRendering

            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                onClicked: {
                    console.log("clicked textRewards")
                    if(ctrlPopups.onePopupIsDisplayed === false)
                    {
                        //popupSelectHiddenCard1.visible = true
                        //ctrlGameBoard.actionClickOnRewards(player)
                        ctrlGameBoard.testAnimation();
                        //popupSelectHiddenCard1.visible = false

                        mouse.accepted = true
                    }
                    else
                        mouse.accepted = false
                }
            }
        }
    }

    Image {
        id: imageHorizontalLineBetweenInfoAndBench
        width: parent.width
        height: 4
        anchors.top: rectangleInfosPackets.bottom
        source: "textures/horizontalLine.png"
    }

    // |-----------------|
    // |      Infos      |
    // |-----------------|
    // |***** Bench *****|
    // |-----------------|
    // |       Hand      |
    // |-----------------|
    ListView {
        id: listViewPacketBench
        objectName: "listViewPacketBench"
        width: (0.6 * height) * count
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: imageHorizontalLineBetweenInfoAndBench.bottom
        anchors.topMargin: 0
        /*anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0*/
        anchors.bottom: listViewPacketHand.top
        anchors.bottomMargin: 10
        orientation: ListView.Horizontal
        preferredHighlightBegin: width/2 - widthComponent/2
        preferredHighlightEnd: width/2 + widthComponent/2
        //highlightRangeMode: ListView.StrictlyEnforceRange
        interactive: false

        property int dragItemIndex: -1
        property int widthComponent: 0.6 * height
        property int heightComponent: height

        spacing: 10
        model: player.bench()
        delegate: Loader {
            id: loaderListViewPacketBench
            //width: 100
            //height: 140

            property int modelIndex: index
            property bool modelIsCard: model.isCard
            property AbstractCard modelCard: model.card === undefined ? null : model.card
            property string modelImageCard: model.imageCard

            sourceComponent: {
                if(modelIsCard === true)
                {
                    //console.log("Loader: componentDelegateCardBench");
                    return componentDelegateCardBench;
                }
                else
                {
                    //console.log("Loader: componentDelegateCardEmptyBench");
                    return componentDelegateCardEmptyBench;
                }
            }
        }

        Component{
            id: componentDelegateCardBench
            Item {
                id: itemDelegatePacketBench
                objectName: "itemDelegatePacketBench"
                x: 5
                width: listViewPacketBench.widthComponent
                height: listViewPacketBench.heightComponent

                Image {
                    id: imageCardInBench
                    objectName: "imageCardInBench"
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: textLifePokemon.top

                    source: modelImageCard
                    fillMode: Image.PreserveAspectFit

                    MouseArea {
                        id: mouseAreaCardBench
                        anchors.fill: parent
                        drag.target: imageCardInBench

                        drag.onActiveChanged: {
                            if (mouseAreaCardBench.drag.active)
                            {
                                listViewPacketBench.dragItemIndex = modelIndex;
                            }
                            imageCardInBench.Drag.drop();
                        }

                        //onReleased: rectangleCardInBench.Drag.drop();

                        onClicked: console.log("componentDelegateCardBench clicked")

                        onPressAndHold: {
                            console.log("componentDelegateCardBench onPressAndHold")
                            popupCardDetailsComplete1.card = modelCard;
                            popupCardDetailsComplete1.visible = true;
                            //player.moveCardFromBenchToFight(modelIndex)

                            //TESTS
                            //ctrlPopups.displayBench(player.bench())
                            //ctrlPopups.displayDeck(player.deck())
                        }
                    }

                    states: [
                        State {
                            when: imageCardInBench.Drag.active
                            ParentChange {
                                target: imageCardInBench
                                parent: boardPlayerArea1
                            }

                            AnchorChanges {
                                target: imageCardInBench
                                //anchors.horizontalCenter: undefined
                                //anchors.verticalCenter: undefined
                                anchors.top: undefined
                                anchors.left: undefined
                                anchors.right: undefined
                                anchors.bottom: undefined
                            }
                        }
                    ]

                    Drag.active: mouseAreaCardBench.drag.active
                    Drag.hotSpot.x: imageCardInBench.width / 2
                    Drag.hotSpot.y: imageCardInBench.height / 2

                    DropArea {
                        id: dropAreaBenchCardBench
                        anchors.fill: parent
                        onDropped: {
                            console.log("onDropped dropAreaBenchCardBench, " + drag.source.objectName);
                            if(drag.source.objectName === "imageCardInHand")
                            {
                                player.moveCardFromHandToBench(listViewPacketHand.dragItemIndex, modelIndex);
                                listViewPacketHand.dragItemIndex = -1;
                            }
                        }
                    }
                }

                Text {
                    id: textLifePokemon
                    height: 25
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: listViewEnergiesByCard.top
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: height
                    text: modelCard.lifeLeft + "/" + modelCard.lifeTotal + " PV"
                }


                ListView {
                    id: listViewEnergiesByCard
                    height: 25
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    orientation: ListView.Horizontal
                    interactive: false

                    //model: listViewPacketBench.model.modelFromCardPokemon(modelIndex)
                    model: modelCard.modelListOfEnergies()
                    delegate:
                        Item {
                        width: 25
                        height: 25

                        Image {
                            id: imageIconEnergy
                            anchors.fill: parent
                            source: model.icon
                        }
                    }

                    Component.onCompleted: console.log("listViewEnergiesByCard is created");
                }
            }
        }

        Component {
            id: componentDelegateCardEmptyBench
            Item {
                id: itemEmptyDelegatePacketBench
                x: 5
                width: listViewPacketBench.widthComponent
                height: listViewPacketBench.heightComponent

                Image {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: rectangleEmptyBarCardEmptyBench.top
                    source: modelImageCard
                    fillMode: Image.PreserveAspectFit
                }

                Rectangle {
                    id: rectangleEmptyBarCardEmptyBench
                    height: 50
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    //color: "green"
                    color: "transparent"
                }

                DropArea {
                    id: dropAreaBenchEmptyCard
                    anchors.fill: parent
                    onEntered: console.log("dropAreaBenchEmptyCard entered");
                    onExited: console.log("dropAreaBenchEmptyCard exited");
                    onDropped: {
                        console.log("onDropped dropAreaBenchEmptyCard");
                        if(drag.source.objectName === "imageCardInHand")
                        {
                            player.moveCardFromHandToBench(listViewPacketHand.dragItemIndex, modelIndex);
                            listViewPacketHand.dragItemIndex = -1;
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("componentDelegateCardEmptyBench clicked")
                        ctrlGameBoard.actionDClickOnBench(player)
                    }
                }
            }
        }
    }

    // |-----------------|
    // |      Infos      |
    // |-----------------|
    // |      Bench      |
    // |-----------------|
    // |****** Hand *****|
    // |-----------------|
    ListView {
        id: listViewPacketHand
        objectName: "listViewPacketHand"
        y: 1728
        width: 110
        height: 80
        contentWidth: 320
        orientation: ListView.Horizontal
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        property int dragItemIndex: -1

        model: player.hand()
        delegate: Item {
            id: itemCardInHand
            objectName: "itemCardInHand"
            width: 80
            height: 160

            Image {
                id: imageCardInHand
                objectName: "imageCardInHand"
                width: 120
                height: 160
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                //anchors.fill: parent

                source: model.imageCard
                fillMode: Image.PreserveAspectFit

                MouseArea {
                    id: mouseAreaCardHand
                    anchors.fill: parent
                    drag.target: imageCardInHand

                    drag.onActiveChanged: {
                        if (mouseAreaCardHand.drag.active)
                        {
                            listViewPacketHand.dragItemIndex = index;
                        }
                        imageCardInHand.Drag.drop();
                    }

                    //onReleased: imageCardInHand.Drag.drop();

                    onPressAndHold: {
                        popupCardDetailsBasic1.card = model.card;
                        popupCardDetailsBasic1.visible = true;
                    }
                }

                states: [
                    State {
                        when: imageCardInHand.Drag.active
                        ParentChange {
                            target: imageCardInHand
                            parent: listViewPacketHand
                        }

                        AnchorChanges {
                            target: imageCardInHand
                            //anchors.horizontalCenter: undefined
                            //anchors.verticalCenter: undefined
                            //anchors.left: undefined
                            anchors.top: undefined
                            anchors.left: undefined
                            anchors.bottom: undefined
                        }
                    }
                ]

                Drag.active: mouseAreaCardHand.drag.active
                Drag.hotSpot.x: imageCardInHand.width / 2
                Drag.hotSpot.y: imageCardInHand.height / 2
            }
        }
    }

}
