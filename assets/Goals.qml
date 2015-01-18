import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2

Page {
    titleBar: TitleBar {
        title: "Sync for Toodledo - Goals"
    }
    
    Container {
        Label {
            id: noItems
            text: "You don't have any goals!"
            visible: listView.dataModel.empty
            horizontalAlignment: HorizontalAlignment.Center
            textStyle.fontSize: FontSize.XLarge
            topMargin: 20
            bottomMargin: 20;
        }
        ListView {
            id: listView
            accessibility.name: "Goal list"
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
                            description: itemContainer.ListItem.view.description(
                                        ListItemData.private, ListItemData.archived);
                            status: itemContainer.ListItem.view.status(ListItemData.level);
                            
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
            
            function status(level) {
                if (level == 0) {
                    return "Lifelong";
                } else if (level == 1) {
                    return "Long-term";
                } else if (level == 2) {
                    return "Short-term";
                }
            }
            
            function description(isPrivate, isArchived) {
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
