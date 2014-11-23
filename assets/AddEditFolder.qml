import bb.cascades 1.2

Page {
    id: addEditFolderPage
    accessibility.name: "Add/edit folder page"
    
    property variant data;
    property bool edit;
    
    function setup() {
        if (edit) {
            folderName.text = data.name;
            folderPrivate.checked = data.private;
            folderArchived.checked = data.archived;
            
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
            title: "Add" //Changed to "Save" in setup if editing a folder
            imageSource: "asset:///images/ic_add.png" //Changed in setup if editing a folder
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (!folderName.text) {
                    titleRequired.visible = true;
                    return;
                }
                
                if (edit) {
                    var folderData = {"id": data.id,
                                "name": folderName.text,
                                "private": folderPrivate.checked,
                                "archived": folderArchived.checked};
                    app.editFolder(data, folderData);
                } else {
                    var folderData = {"name": folderName.text,
                                "private": folderPrivate.checked,
                                "archived": folderArchived.checked};
                    app.addFolder(folderData);
                }
                mainNavPane.pop();
            }
        }
    ]
    
    ScrollView {
        accessibility.name: "Add/edit folder page scrollview"
        scrollViewProperties {
            scrollMode: ScrollMode.Vertical
        }
        Container {
            accessibility.name: "Add/edit folder page container"
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
            TextField {
                id: folderName
                hintText: "Folder Name"
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 30
            }
            CheckBox {
                id: folderArchived
                text: "Archived"
                bottomMargin: 30
                visible: edit //only show when editing a folder
            }
            CheckBox {
                id: folderPrivate
                text: "Private"
                bottomMargin: 30
            }
        }
    }
}
