import QtQuick 2.5
import QtGraphicalEffects 1.0

Item {
    id: root
    width:300
    height: 300

    Rectangle{
        id:image
        anchors.fill: parent
        color: "transparent"

        RotationAnimation on rotation {
            id:animation
            loops: Animation.Infinite
            from: 0
            to: 360
            duration: 20000
            running: true
        }

        Image {
           id: sourceimage
           source: "image://myprovider/" + self.currentIndex
           width: 300
           height: 300
           anchors.fill: parent
           anchors.centerIn: parent
           sourceSize: Qt.size(parent.width, parent.height)
           smooth: true
           visible: false
        }

        Rectangle {
           id: mask
           width: parent.width
           height: parent.height
           radius: height/2
           color:"red"
           visible: false

        }

        OpacityMask {
           anchors.fill: sourceimage
           source: sourceimage
           maskSource: mask
        }

    }
}
