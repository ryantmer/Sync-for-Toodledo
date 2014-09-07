import bb.cascades 1.2

Sheet {
    id: aboutSheet
    
    Page {
        titleBar: TitleBar {
            dismissAction: ActionItem {
                title: "Close"
                onTriggered: {
                    aboutSheet.close();
                }
            }
        }
        Container {
            id: aboutContent
            layout: DockLayout {}
            
            Container {
                layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                Label {
                    text: "ToodleDo10"
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Label {
                    text: "Created by Ryan Mahler (ryantmer)"
                    textStyle.base: SystemDefaults.TextStyles.PrimaryText
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Button {
                    text: "Support"
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    onClicked: {
                        supportDialog.open();
                    }
                }
            }
        }
    
        attachedObjects: [
            Dialog {
                id: supportDialog
                onOpenedChanged: {
                    aboutContent.opacity = 0.1
                }
                onClosed: {
                    aboutContent.opacity = 1.0
                }
                Container {
                    background: Color.LightGray
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    layout: DockLayout {}
                    leftPadding: 20
                    rightPadding: 20
                    topPadding: 20
                    bottomPadding: 20
                    
                    Container {
                        layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
                        Label {
                            text: "Email ryantmer@gmail.com for help!"
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                        Button {
                            text: "Close"
                            horizontalAlignment: HorizontalAlignment.Center
                            onClicked: {
                                supportDialog.close();
                            }
                        }
                    }
                }
            }
        ]
    }
}