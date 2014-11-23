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
                            description: folderItemContainer.ListItem.view.privateOrArchived(
                                        ListItemData.private, ListItemData.archived)
                            
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
            
            function privateOrArchived(isPrivate, isArchived) {
                if (isPrivate == 1 && isArchived == 1) {
                    return "Archived, Private";
                } else if (isPrivate == 1) {
                    return "Private, Not Archived";
                } else if (isArchived == 1) {
                    return "Archived, Not Private";
                } else {
                    return "Not Archived, Not Private";
                }
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
