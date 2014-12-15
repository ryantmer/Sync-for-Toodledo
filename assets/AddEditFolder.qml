import bb.cascades 1.2

Page {
    id: addEditFolderPage
    
    property variant data;
    property bool edit;
    
    function setup() {
        if (edit) {
            nameField.text = data.name;
            privateCheckBox.checked = data.private;
            archivedCheckBox.checked = data.archived;
            
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
                if (!nameField.text) {
                    titleRequired.visible = true;
                    return;
                }
                if (nameField.text.length > 32) {
                    nameTooLong.visible = true;
                    return;
                }
                
                var newData = {}
                if (edit) {
                    newData = data;
                }
                newData["name"] = nameField.text
                newData["private"] = privateCheckBox.checked + 0;
                if (edit) {
                    newData["archived"] = archivedCheckBox.checked + 0;
                    app.folderDataModel.edit(data, newData);
                } else {
                    app.folderDataModel.add(newData);
                }
                mainNavPane.pop();
            }
        }
    ]
    
    ScrollView {
        accessibility.name: "Add/Edit folder scrollview"
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
                id: nameField
                hintText: "Folder Name"
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 30
            }
            CheckBox {
                id: archivedCheckBox
                text: "Archived"
                bottomMargin: 30
                visible: edit //only show when editing a folder
            }
            CheckBox {
                id: privateCheckBox
                text: "Private"
                bottomMargin: 30
            }
        }
    }
}
