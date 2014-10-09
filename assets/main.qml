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
                        Container {
                            id: taskItemContainer
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            leftPadding: 10.0
                            
                            CheckBox {
                                checked: ListItemData.completed
                                verticalAlignment: VerticalAlignment.Center
                                
                                onCheckedChanged: {
                                    if (checked) {
                                        var data = {"id": parseInt(ListItemData.id),
                                                    "completed": Math.floor((new Date()).getTime() / 1000),
                                                    "title": ListItemData.title,
                                                    "duedate": ListItemData.duedate,
                                                    "note": ListItemData.note};
                                        app.editTask(data);
                                    } else {
                                        var data = {"id": parseInt(ListItemData.id),
                                            "completed": 0,
                                            "title": ListItemData.title,
                                            "duedate": ListItemData.duedate,
                                            "note": ListItemData.note};
                                        app.editTask(data);
                                    }
                                }
                            }
                            StandardListItem {
                                title: ListItemData.title
                                status: taskItemContainer.ListItem.view.dueDateString(ListItemData.duedate);
                                description: ListItemData.note;
                                textFormat: TextFormat.Auto
                                
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
                    }
                ]
                
                onTriggered: {
                    var p = editTaskSheetDefinition.createObject();
                    var selectedTask = dataModel.data(indexPath);
                    p.taskId = selectedTask.id;
                    p.completed = selectedTask.completed;
                    p.title = selectedTask.title;
                    
                    if (selectedTask.duedate == 0) {
                        p.noDueDate = true;
                        p.dueDateEnabled = false;
                    } else {
                        p.duedate = app.unixTimeToDateTime(selectedTask.duedate);
                        p.noDueDate = false;
                        p.dueDateEnabled = true;
                    }
                    
                    if (selectedTask.note) {
                        p.note = selectedTask.note;
                    }
                    
                    p.open();
                }
                
                function dueDateString(dueDate) {
                    if (dueDate == 0) {
                        return "No due date";
                    } else if (dueDate <= Math.floor((new Date()).getTime() / 1000)) {
                        return "<html><body style=\"color:red\"><b>" +
                                    app.unixTimeToDateTime(dueDate).toDateString() +
                                    "</b></body></html>";
                    } else {
                        return app.unixTimeToDateTime(dueDate).toDateString();
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
