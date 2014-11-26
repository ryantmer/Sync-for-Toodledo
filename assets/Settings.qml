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
                    settingsSheet.close();
                }
            }
        }
        Container {
            accessibility.name: "Settings page container"
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            topPadding: 20
            leftPadding: 20
            bottomPadding: 20
            rightPadding: 20
            
            Container {
                topPadding: 10
                bottomPadding: 10
                leftPadding: 10
                rightPadding: 10
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                Label {
                    text: "Log out current user"
                    textStyle.fontSize: FontSize.XLarge
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