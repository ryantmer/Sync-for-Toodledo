import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2

NavigationPane {
    id: mainNavPane
    objectName: "mainNavPane"
    
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                settingsSheetDefinition.createObject().open();
            }
        }
        
        actions: [
            ActionItem {
                title: "About"
                imageSource: "asset:///images/ic_info.png"
                onTriggered: {
                    aboutSheetDefinition.createObject().open();
                }
            }
        ]
    }
    
    Page {
        titleBar: TitleBar {
            title: "ToodleDo10"
        }
        
        Container {
            ListView {
                id: taskListView
                accessibility.name: "Task List"
                layout: StackListLayout {
                    headerMode: ListHeaderMode.Sticky
                }
                horizontalAlignment: HorizontalAlignment.Fill
                
                dataModel: app.dataModel
                
                listItemComponents: [
                    ListItemComponent {
                        type: "task"
                        StandardListItem {
                            id: taskItem
                            title: ListItemData.title
                            status: taskItem.ListItem.view.dueDateString(ListItemData.duedate);
                            description: ListItemData.note;
                            
                            contextActions: [
                                ActionSet {
                                    DeleteActionItem {
                                        onTriggered: {
                                            var taskData = {"id": ListItemData.id}
                                            app.removeTask(taskData);
                                        }
                                    }
                                }
                            ]
                        }
                    }
                ]
                
                onTriggered: {
                    var p = editTaskSheetDefinition.createObject();
                    var selectedTask = dataModel.data(indexPath);
                    p.taskId = selectedTask.id;
                    p.completed = selectedTask.completed;
                    p.title = selectedTask.title;
                    p.duedate = app.unixTimeToDateTime(selectedTask.duedate);
                    if (selectedTask.note) {
                        p.note = selectedTask.note;
                    }
                    p.open();
                }
                
                function dueDateString(dueDate) {
                    if (dueDate == 0) {
                        return "No due date";
                    } else {
                        return "Due " + app.unixTimeToDateTime(dueDate).toDateString();
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
                    app.refresh();
                }
            },
            ActionItem {
                title: "Add Task"
                ActionBar.placement: ActionBarPlacement.OnBar
                imageSource: "asset:///images/ic_add.png"
                onTriggered: {
                    var p = addTaskSheetDefinition.createObject();
                    p.open();
                }
            }
        ]
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: addTaskSheetDefinition
            content: AddTask {}
        },
        ComponentDefinition {
            id: editTaskSheetDefinition
            content: EditTask {}
        },
        ComponentDefinition {
            id: settingsSheetDefinition
            content: Settings {}
        },
        ComponentDefinition {
            id: aboutSheetDefinition
            content: About {}
        }
    ]
    
    onPopTransitionEnded: {
        page.destroy();
    }
}
