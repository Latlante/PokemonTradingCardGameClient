import QtQuick 2.0

Flipable {
    id: flipable

    property bool flipped: false
    property alias imagePokemon: imageBack.source

    front: Image {
        source: "back.png"
        fillMode: Image.PreserveAspectFit
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
    }
    back: Image {
        id: imageBack
        source: "back.png"
        fillMode: Image.PreserveAspectFit
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
    }

    transform: Rotation {
        id: rotation
        origin.x: flipable.width/2
        origin.y: flipable.height/2
        axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
        angle: 0    // the default angle
    }

    states: State {
        name: "back"
        PropertyChanges { target: rotation; angle: 180 }
        when: flipable.flipped
    }

    transitions: Transition {
        NumberAnimation { target: rotation; property: "angle"; duration: 1000 }
    }

    /*MouseArea {
        anchors.fill: parent
        onClicked: flipable.flipped = !flipable.flipped
    }*/
}


