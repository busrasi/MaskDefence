import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtMultimedia 5.15

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Mask Defence")

    // CAMERA
    Camera {
        id: camera
        captureMode: Camera.CaptureStillImage
        focus {
            focusMode: CameraFocus.FocusContinuous
            focusPointMode: CameraFocus.FocusPointAuto
        }
        position: Camera.FrontFace
    }

    // CAMERA OUTPUT
    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        source: camera
        focus: visible

        autoOrientation: true
        visible: true
        //fillMode: VideoOutput.PreserveAspectCrop;
        filters: [maskDetectionFilter]
    }

    Connections {
        target: maskDetectionFilter
        function onObjectDetected(result)
        {
            if(result === true) {
                // do for true
            }
            else {
                // do for true
            }
        }
    }

}
