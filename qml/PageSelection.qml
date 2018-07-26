import QtQuick 2.0
import model 1.0

Item {
    id: pageSelection1
    width: 720
    height: 480

    property alias title: textTheme.text

    Rectangle {
        id: background
        anchors.fill: parent

        color: "#2b95ff"

        Rectangle {
            id: rectangleContainerListView
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            anchors.fill: parent
            anchors.margins: 0

            Text {
                id: textTheme
                text: listViewSelection.model.nameSelection
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 20
            }

            ListView {
                id: listViewSelection
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: boutonOk.top
                anchors.bottomMargin: 0
                anchors.top: textTheme.bottom
                anchors.topMargin: 0

                model: ctrlGameBoard.modelListSelect()
                delegate: Rectangle {
                    width: parent.width
                    height: 30
                    color: index == listViewSelection.currentIndex ? "lightgray" : "white"

                    Text {
                        id: textNameSelection
                        anchors.fill: parent
                        font.pixelSize: 20
                        color: "black"
                        text: model.name

                        MouseArea {
                            id: mouseAreaNameSelection
                            anchors.fill: parent
                            onClicked: listViewSelection.currentIndex = index
                        }
                    }
                }
            }




            ButtonStyleGold {
                id: boutonOk

                width: parent.width / 2
                height: 40
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                text: qsTr("Ok")

                onClicked: {
                    console.log("index:" + listViewSelection.currentIndex)
                    //ctrlGameBoard.validatePageSelection(listViewSelection.model.data(listViewSelection.currentIndex))
                    ctrlGameBoard.validatePageSelection(listViewSelection.model.element(listViewSelection.currentIndex))
                }
            }




            ButtonStyleGold {
                id: boutonCancel

                height: 40
                anchors.left: boutonOk.right
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.bottom: parent.bottom
                text: qsTr("Cancel")

                onClicked: ctrlGameBoard.returnToTheMenu()
            }



        }



    }
}
