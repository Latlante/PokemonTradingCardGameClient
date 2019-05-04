import QtQuick 2.0
import model 1.0

Item {
    id: boardFightingArea1
    width: 1000
    height: 600

    property real percentageWidthLine: 0.03
    property int marginBetweenFighter: 0

    property Player play1
    property Player play2
    property int posFightX: boardFightingInformation_P1.x
    property int posFightY: boardFightingInformation_P1.y

    Image {
        id: imageBackgroundFighting
        height: 0.5 * parent.height
        width: height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        source: "textures/background-fight-90.png"
    }

    Image {
        id: imageBackgroungHorizontalLineTop
        width: 0.5 * parent.width
        height: percentageWidthLine * imageBackgroundFighting.width
        anchors.top: parent.top
        anchors.left: parent.left
        source: "textures/verticalLine.png"
    }

    Image {
        id: imageBackgroungVerticalLineTop
        width: percentageWidthLine * imageBackgroundFighting.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: imageBackgroundFighting.top
        source: "textures/verticalLine.png"
    }

    Image {
        id: imageBackgroungVerticalLineBottom
        width: percentageWidthLine * imageBackgroundFighting.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: imageBackgroundFighting.bottom
        anchors.bottom: parent.bottom
        source: "textures/verticalLine.png"
    }

    Image {
        id: imageBackgroungHorizontalLineBottom
        width: 0.5 * parent.width
        height: percentageWidthLine * imageBackgroundFighting.width
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        source: "textures/verticalLine.png"
    }

    BoardFightingInformation {
        id: boardFightingInformation_P1
        width: (parent.width / 2) - (marginBetweenFighter / 2)
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        player: play1
    }

    BoardFightingInformation {
        id: boardFightingInformation_P2
        width: (parent.width / 2) - (marginBetweenFighter / 2)
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        rotation: 180

        player: play2
    }
}
