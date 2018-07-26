import QtQuick 2.0
import model 1.0

Item {
    id: popupLineEdit1
    width: 500
    height: 300

    signal clicked(string result)

    Rectangle {
        id: background
        anchors.fill: parent

        color: "#AAAAAAAA"

        MouseArea {
            id: mouseAreaCancel
            anchors.fill: parent
            onClicked: popupLineEdit1.visible = false

        }

        Rectangle {
            id: rectangleContainer
            width: 300
            height: 150
            color: "#aaaaaa"
            border.width: 2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            Column {
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.left: parent.left
                spacing: 5

                Text {
                    width: parent.width
                    text: "Nom de la partie:"
                    font.pixelSize: 20
                    horizontalAlignment: Text.AlignLeft
                }

                Rectangle {
                    id: rectangleContainerLineEdit
                    width: parent.width
                    height: 25
                    color: "#ffffff"
                    border.width: 1

                    TextInput {
                        id: textInputLineEdit
                        text: qsTr("Partie n1")
                        horizontalAlignment: Text.AlignHCenter
                        anchors.fill: parent
                        font.pixelSize: 18
                    }
                }

                ButtonStyleGold {
                    id: boutonOkLineEdit

                    width: parent.width
                    height: 40
                    text: qsTr("Ok")

                    onClicked: {
                        popupLineEdit1.visible = false
                        popupLineEdit1.clicked(textInputLineEdit.text)
                    }
                }
            }
        }



    }

}
