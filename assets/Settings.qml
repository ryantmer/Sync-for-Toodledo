import bb.cascades 1.2
import bb.system 1.2

Sheet {
    id: settingsSheet
    
    onClosed: {
        destroy();
    }
    
    function setup() {
        var index;
        for (index = 0; index < completedDaysDropDown.options.length; index++) {
            if (completedDaysDropDown.options[index].value == propertyManager.completedTaskAge) {
                break;
            }
        }
        completedDaysDropDown.setSelectedIndex(index);
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
                    propertyManager.completedTaskAge = completedDaysDropDown.selectedValue;
                    settingsSheet.close();
                }
            }
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            topPadding: 20
            leftPadding: 20
            bottomPadding: 20
            rightPadding: 20
            
            Container {
                bottomMargin: 30
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
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                bottomMargin: 30
                Label {
                    text: "Get"
                    textStyle.fontSize: FontSize.XLarge
                    verticalAlignment: VerticalAlignment.Center
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                DropDown {
                    id: completedDaysDropDown
                    
                    options: [
                        Option {
                            text: "1"
                            value: 1
                        },
                        Option {
                            text: "2"
                            value: 2
                        },
                        Option {
                            text: "7"
                            value: 7
                        },
                        Option {
                            text: "30"
                            value: 30
                        }
                    ]
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                Label {
                    text: "days of completed tasks"
                    textStyle.fontSize: FontSize.XLarge
                    verticalAlignment: VerticalAlignment.Center
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 6
                    }
                }
            }
        }
    }
}