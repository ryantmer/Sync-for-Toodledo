import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2

Page {
    titleBar: TitleBar {
        title: "Sync for Toodledo - Locations"
    }
    
    Container {
        ListView {
            id: listView
            layout: StackListLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            
            dataModel: app.locationDataModel
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    Container {
                        id: itemContainer
                        StandardListItem {
                            title: ListItemData.name
                            description: itemContainer.ListItem.view.description(ListItemData.description)
                            
                            contextActions: [
                                ActionSet {
                                    DeleteActionItem {
                                        onTriggered: {
                                            deleteConfirmDialog.show();
                                        }
                                    }
                                }
                            ]
                            
                            attachedObjects: [
                                SystemDialog {
                                    id: deleteConfirmDialog
                                    title: "Delete Location"
                                    body: "Are you sure you want to delete this location?"
                                    confirmButton.label: "Delete"
                                    confirmButton.enabled: true
                                    cancelButton.label: "Cancel"
                                    cancelButton.enabled: true
                                    onFinished: {
                                        if (result == SystemUiResult.ConfirmButtonSelection) {
                                            var locationData = {"id": ListItemData.id}
                                            app.locationDataModel.remove(locationData);
                                        }
                                    }
                                }
                            ]
                        }
                    }
                }
            ]
            
            onTriggered: {
                var location = dataModel.data(indexPath);
                var page = addEditLocationDefinition.createObject();
                page.data = location;
                page.edit = true;
                page.setup();
                mainNavPane.push(page);
            }
            
            function description(desc) {
                if (desc.indexOf("\n") > -1) {
                    //Description is multi-line, take first line as description
                    return desc.substring(0, desc.indexOf("\n"));
                } else {
                    //Note is a single line
                    return desc;
                }
            }
        }
    }
    
    actions: [
        ActionItem {
            title: "Refresh"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_reload.png"
            onTriggered: {
                app.locationDataModel.refresh();
            }
        },
        ActionItem {
            title: "Add Location"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_add.png"
            onTriggered: {
                var page = addEditLocationDefinition.createObject();
                page.edit = false;
                page.setup();
                mainNavPane.push(page);
            }
        }
    ]
    
    attachedObjects: [
        ComponentDefinition {
            id: addEditLocationDefinition
            content: AddEditLocation{}
        }
    ]
}
