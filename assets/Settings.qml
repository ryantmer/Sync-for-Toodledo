import bb.cascades 1.2
import bb.system 1.2

Sheet {
    id: settingsSheet
    accessibility.name: "Settings sheet"
    
    onClosed: {
        destroy();
    }
    
    Page {
        accessibility.name: "Settings page"
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
                    propertyManager.showCompletedTasks = showCompletedTasks.checked;
                    propertyManager.advancedMode = advancedMode.checked;
                    settingsSheet.close();
                }
            }
        }
        Container {
            accessibility.name: "Settings page container"
            layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
            
            Container {
                topPadding: 10
                bottomPadding: 10
                leftPadding: 10
                rightPadding: 10
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                Label {
                    text: "Advanced Mode (shows all task fields)"
                    textStyle.fontSize: FontSize.Large
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                ToggleButton {
                    id: advancedMode
                    checked: propertyManager.advancedMode
                    accessibility.name: "Advanced mode toggle"
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                }
            }
            Container {
                topPadding: 10
                bottomPadding: 10
                leftPadding: 10
                rightPadding: 10
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                Label {
                    text: "Show Recently Completed Tasks"
                    textStyle.fontSize: FontSize.Large
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                ToggleButton {
                    id: showCompletedTasks
                    checked: propertyManager.showCompletedTasks
                    accessibility.name: "Show recently completed tasks toggle"
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                }
            }
            Container {
                topPadding: 10
                bottomPadding: 10
                leftPadding: 10
                rightPadding: 10
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                Label {
                    text: "Log out current user"
                    textStyle.fontSize: FontSize.Large
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                Button {
                    id: logoutButton
                    text: "Log Out"
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    onClicked: {
                        app.logout();
                    }
                }
            }
        }
    }
}