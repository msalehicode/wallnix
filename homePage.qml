import QtQuick 2.15
import QtQuick.Controls

Page
{
    anchors.fill: parent
    property string videoMuted: "false"
    property string videLooped: "true"
    Rectangle
    {
        color:"transparent";
        width:parent.width/2;
        height:parent.height/2;
        anchors.centerIn: parent
        Column
        {
            width:parent.width/2;
            height:parent.height;
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

            Rectangle
            {
                color:"transparent"
                width:parent.width/2;
                height:60
                CustomSingleButton
                {
                    setButtonText:"video mute";
                    setButtonBorderColor: "transparent";
                    onButtonClicked:
                    {
                        backend.setVideoMute("true")
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
                    setButtonText:"video unmute";
                    setButtonBorderColor: "transparent";
                    onButtonClicked:
                    {
                        backend.setVideoMute("false")
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
                    setButtonText:"video loop off";
                    setButtonBorderColor: "transparent";
                    onButtonClicked:
                    {
                        backend.setVideoLoop("false")
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
                    setButtonText:"video loop on";
                    setButtonBorderColor: "transparent";
                    onButtonClicked:
                    {
                        backend.setVideoLoop("true")
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
                    setButtonText:"video pause/resume";
                    setButtonBorderColor: "transparent";
                    onButtonClicked:
                    {
                        backend.setVideoPauseResume()
                    }
                }
            }

        }


    }


}
