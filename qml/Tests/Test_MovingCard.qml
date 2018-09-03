import QtQuick 2.0
import model 1.0
import "../BasicComponents"
import QtQuick.Controls 2.3

Item {
    id: test_MovingCard1
    width: 720
    height: 480

    Rectangle {
        id: rectangleContainerButtonBack
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 40
        color: "#AAAAAA"

        Text {
            id: textButtonBack
            anchors.fill: parent
            text: "Back"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        MouseArea {
            id: mouseAreaButtonBack
            anchors.fill: parent
            onClicked: ctrlGameBoard.factory().displayMenuTest()
        }
    }

    Rectangle {
        id: background

        property int marginPoint: 80

        anchors.top: rectangleContainerButtonBack.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "#AAAAAA"

        Rectangle {
            id: rectangleContainerOptions
            width: 150
            color: "#ffffff"
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            Column {
                anchors.fill: parent

                Button {
                    id: buttonRedToGreen
                    width: 0.8 * parent.width
                    height: 30
                    text: qsTr("Rouge vers Vert")
                    onClicked: {
                        movingCard1.x = rectanglePointTopLeft.x
                        movingCard1.y = rectanglePointTopLeft.y
                        movingCard1.positionXEnd = rectanglePointTopRight.x
                        movingCard1.positionYEnd = rectanglePointTopRight.y
                        movingCard1.startAnimation = true
                    }
                }

                Button {
                    id: buttonRedToBlue
                    width: 0.8 * parent.width
                    height: 30
                    text: qsTr("Rouge vers Bleu")
                    onClicked: {
                        movingCard1.x = rectanglePointTopLeft.x
                        movingCard1.y = rectanglePointTopLeft.y
                        movingCard1.positionXEnd = rectanglePointBottomLeft.x
                        movingCard1.positionYEnd = rectanglePointBottomLeft.y
                        movingCard1.startAnimation = true
                    }
                }

                Button {
                    id: buttonRedToYellow
                    width: 0.8 * parent.width
                    height: 30
                    text: qsTr("Rouge vers Jaune")
                    onClicked: {
                        movingCard1.x = rectanglePointTopLeft.x
                        movingCard1.y = rectanglePointTopLeft.y
                        movingCard1.positionXEnd = rectanglePointBottomRight.x
                        movingCard1.positionYEnd = rectanglePointBottomRight.y
                        movingCard1.startAnimation = true
                    }
                }
            }
        }

        Rectangle {
            id: rectanglePointTopLeft
            width: 5
            height: 5
            color: "#ff0000"
            anchors.left: rectangleContainerOptions.right
            anchors.leftMargin: background.marginPoint
            anchors.top: parent.top
            anchors.topMargin: background.marginPoint
        }

        Rectangle {
            id: rectanglePointTopRight
            width: 5
            height: 5
            color: "#00ff00"
            anchors.right: parent.right
            anchors.rightMargin: background.marginPoint
            anchors.top: parent.top
            anchors.topMargin: background.marginPoint
        }

        Rectangle {
            id: rectanglePointBottomLeft
            width: 5
            height: 5
            color: "#0000ff"
            anchors.left: rectangleContainerOptions.right
            anchors.leftMargin: background.marginPoint
            anchors.bottom: parent.bottom
            anchors.bottomMargin: background.marginPoint
        }

        Rectangle {
            id: rectanglePointBottomRight
            width: 5
            height: 5
            color: "#ffff00"
            anchors.right: parent.right
            anchors.rightMargin: background.marginPoint
            anchors.bottom: parent.bottom
            anchors.bottomMargin: background.marginPoint
        }

        MovingCard {
            id: movingCard1
            sourceImage: "qrc:/Images/P_Loader.png"
            visible: false
        }


    }
}
