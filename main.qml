import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 6.5
import QtQuick.Controls.Material

Window {
    id: rootWindow
    visible: false
    width: 400
    height: 300
    title: "Wallnix"
    color: "#222222"
    onClosing: function(event) {
        event.accepted = false;
        rootWindow.visible = false;
    }

    FileDialog {
        id: fileDialog
        currentFolder : "file:///"
        onAccepted: {
            console.log("Selected file:", fileDialog.currentFile)
        }
        onRejected: {
            console.log("Canceled")
        }
    }


    Button {
        text: "Browse File"
        anchors.centerIn: parent
        onClicked: fileDialog.open()
    }


    Button {
        id:startw
        text: "Start Wallpaper"
        onClicked:
        {
            var path = fileDialog.currentFile + "";  // force conversion to string
            if (path.indexOf("file://") === 0) {
                path = path.substring(7);
            }
            wallpaperManager.startWallpaperbyQML(path);
        }
    }

    Button {
        id:stopw
        anchors.top: startw.bottom
        text: "Stop Wallpaper"
        onClicked: wallpaperManager.stopWallpaperbyQML()
    }
}
