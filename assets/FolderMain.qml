import bb.cascades 1.2

Page {
    accessibility.name: "Main folder list page"
    titleBar: TitleBar {
        title: "Manage folders"
    }
    
    Container {
        accessibility.name: "Main folder list page container"
        ListView {
            id: folderListView
            accessibility.name: "Main folder list"
            layout: StackListLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            
            dataModel: app.folderDataModel
            
            listItemComponents: [
                ListItemComponent {
                    type: "folder"
                    Container {
                        id: folderItemContainer
                        accessibility.name: "List item component container"
                        StandardListItem {
                            title: ListItemData.name
                            
                            contextActions: [
                                ActionSet {
                                    DeleteActionItem {
                                        onTriggered: {
                                            var folderData = {"id": ListItemData.id}
                                            app.removeFolder(folderData);
                                        }
                                    }
                                }
                            ]
                        }
                    }
                }
            ]
            
            onTriggered: {
                var folder = dataModel.data(indexPath);
                var page = addEditFolderDefinition.createObject();
                page.data = folder;
                page.edit = true;
                page.setup();
                mainNavPane.push(page);
            }
        }
    }
    
    actions: [
        ActionItem {
            title: "Add Folder"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_add.png"
            onTriggered: {
                var page = addEditFolderDefinition.createObject();
                page.edit = false;
                page.setup();
                mainNavPane.push(page);
            }
        }
    ]
    
    attachedObjects: [
        ComponentDefinition {
            id: addEditFolderDefinition
            content: AddEditFolder{}
        }
    ]
}
