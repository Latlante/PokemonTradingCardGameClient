import QtQuick 2.9

Item {
    id: movingCard
    width: 50
    height: 100

    property alias sourceImage: imageCard.source
    property int positionXEnd: 0
    property int positionYEnd: 0
    property int delay: 0
    property bool startAnimation: ctrlAnimation.movingCardStarted
    signal animationRunningChanged(bool running)

    onStartAnimationChanged:
    {
        console.log("onStartAnimationChanged: " + startAnimation);

        if(startAnimation == true)
        {
            var player = ctrlAnimation.movingCardPlayer;

            if(player === player1)
            {
                movingCard.x = board1.boardPlayer1.textDeck.x
                movingCard.y = board1.boardPlayer1.textDeck.y
                movingCard.positionXEnd = board1.boardPlayer1.listViewPacketHand.x + 50
                movingCard.positionYEnd = board1.boardPlayer1.listViewPacketHand.y + 50
            }
            else
            {
                movingCard.x = boardPlayer2.textDeck.x
                movingCard.y = boardPlayer2.textDeck.y
                movingCard.positionXEnd = boardPlayer2.listViewPacketHand.x + 50
                movingCard.positionYEnd = boardPlayer2.listViewPacketHand.y + 50
            }

            console.log("PositionStart:" + movingCard.x + "." + movingCard.y)
            console.log("PositionEnd:" + movingCard.positionXEnd + "." + movingCard.positionYEnd)

            movingCard.state = "start"
        }
        else
            movingCard.state = ""
    }

    Image {
        id: imageCard
        anchors.fill: parent
    }

    transitions: [
        Transition {
            from: ""
            to: "start"
            SequentialAnimation {

                PauseAnimation {
                    duration: delay
                }
                PropertyAction {
                    target: movingCard
                    properties: "visible"
                    value: true
                }

                ParallelAnimation {
                    PropertyAnimation {
                        target: movingCard
                        easing.type: Easing.InQuad
                        properties: "x"
                        to: positionXEnd
                        duration: 1000
                    }
                    PropertyAnimation {
                        target: movingCard
                        properties: "y"
                        to: positionYEnd
                        duration: 1000
                    }
                }

                PropertyAction {
                    target: movingCard
                    properties: "visible"
                    value: false
                }

                onRunningChanged: {
                    console.log("Animation arrêté")
                    //rectangleBackground.state = ""
                }
            }

            onRunningChanged: {
                console.log("Animation running changed " + running)

                movingCard.animationRunningChanged(running)

                if(running == false)
                   ctrlAnimation.movingCardStarted = false
            }

        }
    ]
}
