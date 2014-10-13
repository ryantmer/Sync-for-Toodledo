import bb.cascades 1.2

Sheet {
    id: aboutSheet
    accessibility.name: "About sheet"
    
    onClosed: {
        destroy();
    }
    
    Page {
        accessibility.name: "About page"
        
        titleBar: TitleBar {
            dismissAction: ActionItem {
                title: "Close"
                onTriggered: {
                    aboutSheet.close();
                }
            }
        }
        Container {
            layout: DockLayout {}
            
            Container {
                id: aboutContent
                accessibility.name: "About page container"
                layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                Label {
                    text: "ToodleDo10"
                    textStyle.fontSize: FontSize.XXLarge
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    bottomMargin: 30
                }
                Label {
                    text: "Created by Ryan Mahler (ryantmer)"
                    textStyle.fontSize: FontSize.Default
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    bottomMargin: 30
                }
                Button {
                    text: "Contact / Help!"
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    bottomMargin: 30
                    
                    onClicked: {
                        emailInvoke.query.uri = "mailto:toodledo10app@gmail.com?subject=Toodledo10%20Help";
                        emailInvoke.query.updateQuery();
                    }
                }
                TextArea {
                    text: "http://toodledo10.ryantmer.com"
                    editable: false
                    inputMode: TextAreaInputMode.Text
                    textStyle.fontSize: FontSize.Default
                    textStyle.textAlign: TextAlign.Center
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    bottomMargin: 30
                }
                
                attachedObjects: [
                    Invocation {
                        id: emailInvoke
                        query.mimeType: "text/plain"
                        query.invokeTargetId: "sys.pim.uib.email.hybridcomposer"
                        query.invokeActionId: "bb.action.SENDEMAIL"
                        onArmed: {
                            emailInvoke.trigger(emailInvoke.query.invokeActionId);
                        }
                    }
                ]
            }
        }
    }
}