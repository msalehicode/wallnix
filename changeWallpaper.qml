import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs 6.5
import QtQuick.Controls.Material

import "."
Page
{
    anchors.fill:parent
    property string newPath: ""
    property string playingPath: "" //will set from outside
    property string errorMessage: ""

    function showPreview(thePath,convertType="convertToFileUrl",
                         resetNewPath=true)
    {
        previewImage.playing=false;

        if(convertType==="convertToFileUrl")
            previewImage.source = convertToFileUrl(thePath);
        else
            previewImage.source = convertToLocalUrl(thePath);

        previewImage.playing = true;

        if(resetNewPath)
            newPath="";
    }

    function convertToLocalUrl(path)
    {
        path += ""; //force conversion to string
        if (path.indexOf("file://") === 0)
        {
            return path.substring(7);
        }
        return path
    }

    function convertToFileUrl(path)
    {
        return "file://"+path;
    }

    FileDialog {
        id: fileDialog
        currentFolder : "file:///"
        nameFilters: [ "GIF Files (*.gif)" ]
        onAccepted:
        {
            //convert to local path to C++ find it from files
            newPath = convertToLocalUrl(fileDialog.currentFile);

            //play as preview
            //convert source path from Local into accepted url for Component
            showPreview(newPath,"convertToFileUrl",false);
        }
        onRejected:
        {
            showPreview(playingPath,"convertToFileUrl",true);
        }
    }

    Rectangle
    {
        color:"#222222"
        width: parent.width
        height:parent.height
        anchors.verticalCenter: parent.verticalCenter
        Rectangle
        {
            id:basePreviewRect
            color:"transparent"
            width: parent.width/1.5
            height: parent.width/2
            border.color: "grey"
            radius:20
            clip:true
            anchors.centerIn: parent
            AnimatedImage
            {
                id:previewImage
                anchors.fill: parent
                playing: true
                source: convertToFileUrl(playingPath)
            }
            Button
            {
                id:browseButton
                text: "Browse File"
                onClicked: fileDialog.open()
                width: 150
                height:50
                anchors.centerIn: parent
            }
        }
        Rectangle
        {
            color:"transparent"
            width: 300
            visible: newPath==="" ? false : true
            height: 50
            anchors.top:basePreviewRect.bottom;
            anchors.horizontalCenter: basePreviewRect.horizontalCenter
            Row
            {
                width:parent.width;
                height:parent.height
                spacing: 10
                Button
                {
                    id:applyChange
                    text: "Apply"
                    width: 150
                    height:50
                    onClicked:
                    {
                        if(newPath!="")
                            wallpaperManager.changeWallpaperbyQML(newPath);
                    }
                }
                Button
                {
                    id:discardChange
                    text: "Discard"
                    width: 150
                    height:50
                    onClicked:
                    {
                        showPreview(playingPath);
                    }
                }
            }

        }



        Rectangle
        {
            id:baseConfirmYesOrNo;
            anchors.fill: parent;
            color:"black";
            opacity: 0.8;
            visible: false;

            MouseArea
            {
                anchors.fill: parent;
                onClicked:
                {
                    dialogMessage.response="ok";
                }
            }
        }
        CustomDialogMessage
        {
            id:dialogMessage;
            visible:baseConfirmYesOrNo.visible
            isSingleButton:true

            //right button is that button we are working with
            textRightButton:"Ok"
            rightButtonBackground:"black"
            rightButtonForeground:"white"
            titleMessage:"Error:"
            textMessage: errorMessage
            onResponseChanged:
            {
                baseConfirmYesOrNo.visible=false;
            }
        }



    }

    Connections
    {
        target: wallpaperManager
        function onResultChangeWallpaperPath(res)
        {
            if(res === "ok")
            {
                playingPath=newPath;
                showPreview(playingPath,"convertToFileUrl",true);
            }
            else
            {
                errorMessage = "could not change wallpapaer becasue of "+res;
                baseConfirmYesOrNo.visible=true;
                showPreview(playingPath);//reset preview
            }
        }
    }



}

