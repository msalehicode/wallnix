import QtQuick 2.15
import QtQuick.Controls

Page
{
    anchors.fill: parent
    Rectangle
    {
        color:"transparent";
        width:parent.width/2;
        height:parent.height/2;
        anchors.centerIn: parent
        Row
        {
            width:parent.width;
            height:parent.height/2;
            spacing: 10

            Rectangle
            {
                color:"transparent"
                width:parent.width/2;
                height:60
                CustomSingleButton
                {
                    id:startWallaper1
                    setButtonText:"start wallaper";
                    setButtonBorderColor: "transparent";
                    onButtonClicked:
                    {
                        backend.startWallpaperbyQML();
                    }
                }
            }
            Rectangle
            {
                color:"transparent"
                width:parent.width/2;
                height:60
                CustomSingleButton
                {
                    id:stopWallpaper2
                    setButtonText:"stop wallaper";
                    setButtonBorderColor: "transparent";
                    onButtonClicked:
                    {
                        backend.stopWallpaperbyQML()
                    }
                }
            }


        }


    }


}
