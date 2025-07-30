import QtQuick 2.15
import QtQuick.Controls 2.15

import QtQuick.Controls.Material

Window {
    id: rootWindow
    visible: false
    width: 700
    height: 700
    title: "Wallnix"
    color: "#222222"
    onClosing: function(event)
    {
        event.accepted = false;
        rootWindow.visible = false;
    }

    Loader
    {
        id:loader
        source:"homePage.qml"
        anchors.fill: parent
        onLoaded:
        {
            if (loader.source.toString() === "changeWallpaper.qml")
                wallpaperManager.getCurrentWallpaper();
        }

    }

    Drawer
    {
        id: drawer
        width: 0.20* parent.width
        height: parent.height
        edge: Qt.LeftEdge
        Column
        {
            anchors.fill: parent
            spacing: 10
            padding: 10
            Button
            {
                text: "Home Page"
                onClicked:
                {
                    loader.source = "homePage.qml"
                    drawer.close()
                }
            }
            Button
            {
                text: "Browse Wallaper"
                onClicked:
                {
                    loader.source = "changeWallpaper.qml"
                    drawer.close()
                }
            }
            Button
            {
                text: "Settings"
                onClicked:
                {
                    loader.source = "settings.qml"
                    drawer.close()
                }
            }
        }
    }

    Rectangle
    {
        color:"transparent"
        anchors.top: parent.top
        anchors.left: parent.left
        width:60;
        height:60
        CustomSingleButton
        {
            id:stopWallpaper2
            setButtonText:"+";
            setButtonBorderColor: "transparent";
            onButtonClicked:
            {
                drawer.open()
            }
        }
    }


    Connections
    {
        target: wallpaperManager
        function onCurrentWallpaperIs(thePath)
        {
            console.log ("current path is="+thePath)
            // After the component is loaded, we can to our property
            loader.item.playingPath = thePath;
        }
    }
}
