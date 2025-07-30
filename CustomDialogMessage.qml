import QtQuick
import "."
Item
{
    id: local_root;
    property bool isSingleButton: false
    property string response : "wait";

    property string textRightButton: "BTN1";
    property string textLeftButton: "BTN2";


    property string rightButtonBackground:"black"
    property string rightButtonForeground:"white"

    property string textMessage : "dialog message";
    property string fontColorText: "black"
    property string textFontSize: "25"

    property string titleMessage : "Are you sure?";
    property string titleFontSize: "30"
    property string fontColorTitle: "black"


    property string leftButtonBackground:"red"
    property string leftButtonForeground:"white"

    property string leftButtonResponseWhenClicked: "left button clicked"
    property string rightButtonResponseWhenClicked: "right button clicked"

    property string backgroundDialog: "white"

    anchors.fill: parent
    Rectangle
    {
        id:baseDialog
        color:backgroundDialog;
        width:parent.width/1.25;

        height: messagelabel.text.length<80 ? 300 :messagelabel.text.length *4 + 65//65 is margin and hieght button

        anchors.centerIn:parent;
        radius:25;
        Text
        {
            id:theTitleTxt
            color:fontColorTitle;
            font.bold: true;
            padding:25;
            visible: titleMessage==="" ? false : true
            font.pointSize: titleFontSize/1.75;
            width:parent.width;
            height:50;
            text:titleMessage;
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 25
        }

        Rectangle
        {
            id:baseLabel;
            width:parent.width/2;
            height:parent.height/2;
            color:"transparent";
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: theTitleTxt.bottom;
            anchors.topMargin: 15
            clip:true;
            Text
            {
                id:messagelabel;
                color:fontColorText;
                font.bold: true;
                // padding:25;
                font.pointSize: titleFontSize/1.75;
                anchors.fill: parent
                text:textMessage;
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }


        Rectangle
        {
            id:baseButtons;
            width: isSingleButton==true ? parent.width/2 : parent.width/1.25;
            height:40;
            color:"transparent"
            anchors
            {
                bottom: parent.bottom;
                bottomMargin:25;
                horizontalCenter:parent.horizontalCenter;
            }
            clip:true;
            CustomSingleButton
            {
                id:singelButton
                visible: isSingleButton==true ? true : false;
                setButtonText:textRightButton;
                setButtonBackColor: rightButtonBackground
                setButtonFontColor: rightButtonForeground;
                setButtonBorderColor: "transparent";
                onButtonClicked:
                {
                    response=rightButtonResponseWhenClicked;
                }
            }

            CustomButton
            {
                id:doubleButton;
                visible: isSingleButton==false ? true : false;
                setRightButtonText:textRightButton;
                setRightButtonBackColor: rightButtonBackground
                setRightButtonFontColor: rightButtonForeground;
                setRightButtonBorderColor: "transparent";

                setLeftButtonText: textLeftButton;
                setLeftButtonBackColor: leftButtonBackground
                setLeftButtonFontColor: leftButtonForeground
                setLeftButtonBorderColor: "transparent";

                onLeftButtonClicked:
                {
                    response=leftButtonResponseWhenClicked;
                }
                onRightButtonClicked:
                {
                    response=rightButtonResponseWhenClicked;
                }
            }
        }

    }

}
