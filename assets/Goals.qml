import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2

Page {
    titleBar: TitleBar {
        title: "Sync for Toodledo - Goals"
    }
    
    Container {
        ListView {
            id: listView
            layout: StackListLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            
            dataModel: app.goalDataModel
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    Container {
                        id: itemContainer
                        StandardListItem {
                            title: ListItemData.name
                            description: itemContainer.ListItem.view.description(ListItemData.note);
                            status: itemContainer.ListItem.view.status();
                            
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
                                    title: "Delete Goal"
                                    body: "Are you sure you want to delete this goal?"
                                    confirmButton.label: "Delete"
                                    confirmButton.enabled: true
                                    cancelButton.label: "Cancel"
                                    cancelButton.enabled: true
                                    onFinished: {
                                        if (result == SystemUiResult.ConfirmButtonSelection) {
                                            var goalData = {"id": ListItemData.id}
                                            app.goalDataModel.remove(goalData);
                                        }
                                    }
                                }
                            ]
                        }
                    }
                }
            ]
            
            onTriggered: {
                var goal = dataModel.data(indexPath);
                var page = addEditGoalDefinition.createObject();
                page.data = goal;
                page.edit = true;
                page.setup();
                mainNavPane.push(page);
            }
            
            function status() {
                
            }
            
            function description(note) {
                if (note.indexOf("\n") > -1) {
                    //Note is multi-line, take first line as description
                    return note.substring(0, note.indexOf("\n"));
                } else {
                    //Note is a single line
                    return note;
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
                app.goalDataModel.refresh();
            }
        },
        ActionItem {
            title: "Add Goal"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_add.png"
            onTriggered: {
                var page = addEditGoalDefinition.createObject();
                page.edit = false;
                page.setup();
                mainNavPane.push(page);
            }
        }
    ]
    
    attachedObjects: [
        ComponentDefinition {
            id: addEditGoalDefinition
            content: AddEditGoal{}
        }
    ]
}
