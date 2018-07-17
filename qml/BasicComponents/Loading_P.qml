import QtQuick 2.0

Item {
    width: 100
    height: 100

    Image {
        id: imageToRotate
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.bottomMargin: 5
        anchors.topMargin: 5
        rotation: 0
        fillMode: Image.PreserveAspectFit
        anchors.fill: parent
        source: "qrc:/Images/P_Loader.png"

        SequentialAnimation {
            id: rotationYAnimation
            loops: Animation.Infinite
            PropertyAnimation {
                target: imageToRotate
                properties: "rotation"
                from: 0
                to: 360
                duration: 3000
            }
        }

        Component.onCompleted: rotationYAnimation.start()
    }

}
