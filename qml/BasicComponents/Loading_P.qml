import QtQuick 2.0

Item {
    width: 100
    height: 100

    Image {
        id: image
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.bottomMargin: 5
        anchors.topMargin: 5
        rotation: 0
        fillMode: Image.PreserveAspectFit
        anchors.fill: parent
        source: "qrc:/Images/P_Loader.png"

        Behavior on rotation {
            id: behaviorRotation
            NumberAnimation {
                target: image
                duration: 2000
            }
        }

        Timer {
            id: timerRotation
            interval: 3000
            repeat: true
            running: true
            onTriggered: {
                behaviorRotation.enabled = false;
                image.rotation = 0;
                behaviorRotation.enabled = true
                image.rotation = 360;
            }
        }
    }

}
