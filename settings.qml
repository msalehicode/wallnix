import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Page
{
    anchors.fill: parent
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
                CheckBox
                {
                    id:start_onStartup
                    checked: false
                    text: "start on system Startup?"
                }
            }
            Rectangle
            {
                color:"transparent"
                width:parent.width/2;
                height:60
                CheckBox
                {
                    id:auto_pause
                    checked: false
                    text: "auto pause when a window is Maximumed/Fullscreend?"
                }
            }

            Rectangle
            {
                color:"transparent"
                width:parent.width/2;
                height:60
                CustomSingleButton
                {
                    id:saveButton
                    setButtonText:"save";
                    setButtonBorderColor: "transparent";
                    onButtonClicked:
                    {
                        console.log("clciked on apply changes to settings")
                        backend.setRunOnStartUp(start_onStartup.checked);
                        backend.setAutoPause(start_onStartup.checked);
                    }
                }
            }

        }
    }
    Component.onCompleted:
    {
        //run on start up
        var res_startup = backend.getSettingValue("is_startup_enabled");
        var isStartupEnabled = false;
        if (typeof res_startup === "boolean")
            isStartupEnabled = res_startup;
        else if (typeof res_startup === "string")
        {
            var trimmed = res_startup.trim().toLowerCase();
            isStartupEnabled = (trimmed === "true" || trimmed === "1");
        }
        start_onStartup.checked = isStartupEnabled;



        //auto pause
        var res_autopause = backend.getSettingValue("is_auto_pause_enabled");
        var isAutoPauseEnabled = false;
        if (typeof res_autopause === "boolean")
            isAutoPauseEnabled = res_autopause;
        else if (typeof res_autopause === "string")
        {
            trimmed = res_autopause.trim().toLowerCase();
            isAutoPauseEnabled = (trimmed === "true" || trimmed === "1");
        }
        auto_pause.checked = isAutoPauseEnabled;
    }

}
