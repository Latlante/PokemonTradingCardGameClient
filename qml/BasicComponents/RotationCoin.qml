import QtQuick 2.0

Item {
    id: rotationCoin1

    property string imageFront: "qrc:/Images/pile.png"
    property string imageBack: "qrc:/Images/face.png"
    property int result: 1

    width: 100
    height: 100

    function startAnimation()
    {
        if(rotationCoin1.state != "start")
        {
            if(result == 0)
            {
                animationRotation.image1 = imageFront
                animationRotation.image2  = imageBack
            }
            else
            {
                animationRotation.image1 = imageBack
                animationRotation.image2  = imageFront
            }

            rotationCoin1.state = "start"
        }
    }

    Image {
        id: imageToRotate
        anchors.fill: parent
        antialiasing: true
        source: rotationCoin1.imageFront
        transform: Rotation {
            id: rotationY
            property real angleRotationY: 0
            origin.x: width / 2
            origin.y: height / 2
            axis {
                x: 1
                y: 0
                z: 0
            }
            angle: angleRotationY
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: startAnimation()
    }

    transitions: [
        Transition {
            from: ""
            to: "start"

            SequentialAnimation {
                id: rotationYAnimation
                //loops: Animation.Infinite
                PauseAnimation {
                    duration: 0
                }

                //property int speedRotation: 2000
                /*loops: 5

                PropertyAction {
                    target: imageToRotate
                    properties: "source"
                    value: rotationCoin1.imageFront
                }
                PropertyAnimation {
                    target: rotationY
                    properties: "angleRotationY"
                    from: 0
                    to: 90
                    duration: 500
                }
                PropertyAction {
                    target: imageToRotate
                    properties: "source"
                    value: rotationCoin1.imageBack
                }
                PropertyAnimation {
                    target: rotationY
                    properties: "angleRotationY"
                    from: -90
                    to: 90
                    duration: 1000
                }
                PropertyAction {
                    target: imageToRotate
                    properties: "source"
                    value: rotationCoin1.imageFront
                }
                PropertyAnimation {
                    target: rotationY
                    properties: "angleRotationY"
                    from: -90
                    to: 0
                    duration: 500
                }*/

                ParallelAnimation {
                    //Rotation
                    SequentialAnimation {
                        id: animationRotation
                        loops: 11
                        property string image1
                        property string image2
                        PropertyAction {
                            target: imageToRotate
                            properties: "source"
                            value: animationRotation.image1
                        }
                        PropertyAnimation {
                            target: rotationY
                            properties: "angleRotationY"
                            from: 0
                            to: 90
                            duration: 50
                        }
                        PropertyAction {
                            target: imageToRotate
                            properties: "source"
                            value: animationRotation.image2
                        }
                        PropertyAnimation {
                            target: rotationY
                            properties: "angleRotationY"
                            from: -90
                            to: 90
                            duration: 100
                        }
                        PropertyAction {
                            target: imageToRotate
                            properties: "source"
                            value: animationRotation.image1
                        }
                        PropertyAnimation {
                            target: rotationY
                            properties: "angleRotationY"
                            from: -90
                            to: 0
                            duration: 50
                        }
                    }

                    //Jump
                    SequentialAnimation {
                        PropertyAnimation {
                            target: rotationCoin1
                            easing.type: Easing.OutQuad
                            properties: "y"
                            to: rotationCoin1.y - 300
                            duration: 1000
                        }
                        PropertyAnimation {
                            target: rotationCoin1
                            easing.period: 0.7
                            easing.type: Easing.InQuad
                            properties: "y"
                            to: rotationCoin1.y
                            duration: 1200
                        }
                    }
                }

            }

            onRunningChanged: {
                if(!rotationYAnimation.running)
                    rotationCoin1.state = ""
            }
        }
    ]
}
