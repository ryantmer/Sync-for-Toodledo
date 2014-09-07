import bb.cascades 1.2
import bb.system 1.2

Sheet {
    id: settingsSheet
    
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
                topPadding: 20
                bottomPadding: 20
                leftPadding: 20
                rightPadding: 20
                text: "Show Time in Add/Edit Tasks"
                checked: propertyManager.showTaskTime
            }
            CheckBox {
                id: advancedModeCheckbox
                topPadding: 20
                bottomPadding: 20
                leftPadding: 20
                rightPadding: 20
                text: "Advanced Mode (shows all task fields)"
                checked: propertyManager.advancedMode
            }
            Button {
                id: removeLocalTasksButton
                topPadding: 20
                bottomPadding: 20
                leftPadding: 20
                rightPadding: 20
                objectName: "removeLocalTasksButton"
                text: "Clear Local Task Storage"
                onClicked: {
                    cacheClearedToast.show();
                }
            }
        }
        
        attachedObjects: [
            SystemToast {
                id: cacheClearedToast
                body: "Cache Cleared!"
            }
        ]
    }
}