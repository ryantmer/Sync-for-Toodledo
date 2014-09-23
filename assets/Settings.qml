import bb.cascades 1.2
import bb.system 1.2

Sheet {
    id: settingsSheet
    
    onClosed: {
        destroy();
    }
    
    Page {
        titleBar: TitleBar {
            dismissAction: ActionItem {
                title: "Cancel"
                onTriggered: {
                    settingsSheet.close();
                }
            }
            acceptAction: ActionItem {
                title: "Save"
                onTriggered: {
                    propertyManager.showTaskTime = showTimeCheckbox.checked;
                    propertyManager.advancedMode = advancedModeCheckbox.checked;
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
            }
            CheckBox {
                id: advancedModeCheckbox
                text: "Advanced Mode (shows all task fields)"
                checked: propertyManager.advancedMode
            }
            Button {
                id: logoutButton
                text: "Log Out"
                onClicked: {
                    app.logout();
                }
            }
        }
    }
}