import bb.cascades 1.2

Page {
    id: addEditContextPage
    
    property variant data;
    property bool edit;
    
    function setup() {
        if (edit) {
            contextName.text = data.name;
            contextPrivate.checked = data.private;
            
            addSaveButton.title = "Save";
            addSaveButton.imageSource = "asset:///images/ic_save.png";
        }
    }
    
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: "Cancel"
            onTriggered: {
                mainNavPane.pop();
            }
        }
    }
    
    actions: [
        ActionItem {
            id: addSaveButton
            title: "Add" //Changed to "Save" in setup if editing a context
            imageSource: "asset:///images/ic_add.png" //Changed in setup if editing a context
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (!contextName.text) {
                    titleRequired.visible = true;
                    return;
                }
                if (contextName.text.length > 32) {
                    nameTooLong.visible = true;
                    return;
                }
                
                if (edit) {
                    var contextData = {"id": data.id,
                                "name": contextName.text,
                                "private": contextPrivate.checked + 0};
                    app.contextDataModel.edit(data, contextData);
                } else {
                    var contextData = {"name": contextName.text,
                                "private": contextPrivate.checked + 0};
                    app.contextDataModel.add(contextData);
                }
                mainNavPane.pop();
            }
        }
    ]
    
    ScrollView {
        scrollViewProperties {
            scrollMode: ScrollMode.Vertical
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            topPadding: 20
            leftPadding: 20
            rightPadding: 20
            bottomPadding: 20
            
            Label {
                id: titleRequired
                text: "Required"
                textStyle.color: Color.Red
                visible: false
            }
            Label {
                id: nameTooLong
                text: "Max 32 characters"
                textStyle.color: Color.Red
                visible: false
            }
            TextField {
                id: contextName
                hintText: "Folder Name"
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 30
            }
            CheckBox {
                id: contextPrivate
                text: "Private"
                bottomMargin: 30
            }
        }
    }
}
