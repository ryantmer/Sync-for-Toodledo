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
                    propertyManager.showCompletedTasks = showCompletedTasksCheckbox.checked;
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
                id: advancedModeCheckbox
                text: "Advanced Mode (shows all task fields)"
                checked: propertyManager.advancedMode
            }
            CheckBox {
                id: showCompletedTasksCheckbox
                text: "Show Completed Tasks"
                checked: propertyManager.showCompletedTasks
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