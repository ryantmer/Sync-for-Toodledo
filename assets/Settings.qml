import bb.cascades 1.2
import bb.system 1.2

Page {
    titleBar: TitleBar {
        dismissAction: ActionItem {
            title: "Close"
            onTriggered: {
                settingsSavedToast.show();
                settingsSheet.close();
            }
        }
    }
    Container {
        layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
        topPadding: 10
        bottomPadding: 10
        leftPadding: 10
        rightPadding: 10
        
        CheckBox {
            id: showTimeCheckbox
            text: "Show Time in Add/Edit Tasks"
            checked: propertyManager.showTaskTime
            onCheckedChanged: {
                propertyManager.showTaskTime = showTimeCheckbox.checked 
            }
        }
        CheckBox {
            id: advancedModeCheckbox
            text: "Advanced Mode (shows all task fields)"
            checked: propertyManager.advancedMode
            onCheckedChanged: {
                propertyManager.advancedMode = advancedModeCheckbox.checked
            }
        }
    }
    
    attachedObjects: [
        SystemToast {
            id: settingsSavedToast
            body: "Settings Saved!"
        }
    ]
}
