import QtQuick 2.0

Item {
    id: menuTest1
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
            onClicked: ctrlGameBoard.returnToTheMenu()
        }
    }

    Rectangle {
        id: background
        color: "#808080"
        border.color: "#00000000"
        anchors.top: rectangleContainerButtonBack.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        ListView {
            id: listViewAllAnimation
            anchors.fill: parent

            model: ctrlGameBoard.modelTestAnimation()
            delegate: Item {
                width: parent.width
                height: 30

                Rectangle {
                    id: rectangleContainerDelegate
                    anchors.fill: parent
                    //color: "#ffc90e"
                    color: "white" //listViewAllAnimation.currentIndex == index ? "white" : "#60ffff"
                    //border.color: "#0000ff"
                    border.color: "black"
                    border.width: 2

                    Text {
                        id: textDelegateInfo
                        //anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.right: parent.right
                        font.pixelSize: 14
                        font.bold: true
                        color: "black"
                        text: model.name;
                    }

                    MouseArea {
                        id: mouseAreaDelegateAnimation
                        anchors.fill: parent
                        onClicked: {
                            //listViewAllAnimation.currentIndex = index
                            ctrlGameBoard.factory().displayCustomLink(model.link)
                        }
                    }
                }
            }
        }
    }
}
