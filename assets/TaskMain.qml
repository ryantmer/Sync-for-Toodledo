import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2

NavigationPane {
    id: mainNavPane
    objectName: "mainNavPane"
    accessibility.name: "Navigation pane for main task list"
    
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
        accessibility.name: "Main task list page"
        titleBar: TitleBar {
            title: "Sync for Toodledo - All Tasks"
        }
        
        Container {
            accessibility.name: "Main task list page container"
            
            ListView {
                id: taskListView
                accessibility.name: "Main task list"
                layout: StackListLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                
                dataModel: app.taskDataModel
                
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        Container {
                            id: taskItemContainer
                            accessibility.name: "List item component container"
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            leftPadding: 10.0
                            
                            CheckBox {
                                checked: ListItemData.completed
                                verticalAlignment: VerticalAlignment.Center
                                
                                onCheckedChanged: {
                                    if (checked) {
                                        var oldData = {"id": parseInt(ListItemData.id),
                                                "completed": ListItemData.completed};
                                        var newData = {"id": parseInt(ListItemData.id),
                                                    "completed": Math.floor((new Date()).getTime() / 1000)};
                                        app.editTask(oldData, newData);
                                    } else {
                                        var oldData = {"id": parseInt(ListItemData.id),
                                            "completed": ListItemData.completed};
                                        var newData = {"id": parseInt(ListItemData.id),
                                            "completed": 0};
                                        app.editTask(oldData, newData);
                                    }
                                }
                                accessibility.name: "completed"
                                accessibility.description: "Checked when task is completed"
                            }
                            StandardListItem {
                                title: ListItemData.title
                                status: taskItemContainer.ListItem.view.dueDateString(ListItemData.duedate);
                                description: taskItemContainer.ListItem.view.descriptionString(ListItemData.note);
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
                    var task = dataModel.data(indexPath);
                    var page = addEditTaskDefinition.createObject();
                    page.data = task;
                    page.edit = true;
                    page.setup();
                    mainNavPane.push(page);
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
                
                function descriptionString(note) {
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
                    app.refreshTasks();
                }
            },
            ActionItem {
                title: "Add Task"
                ActionBar.placement: ActionBarPlacement.OnBar
                imageSource: "asset:///images/ic_add.png"
                onTriggered: {
                    var page = addEditTaskDefinition.createObject();
                    page.edit = false;
                    page.setup();
                    mainNavPane.push(page);
                }
            },
            ActionItem {
                title: "Manage Folders"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/ic_settings.png"
                onTriggered: {
                    var page = manageFoldersDefinition.createObject();
                    mainNavPane.push(page);
                }
            },
            ActionItem {
                title: "Completed Tasks"
                ActionBar.placement: ActionBarPlacement.InOverflow
                onTriggered: {
                    var page = completedTasksDefinition.createObject();
                    mainNavPane.push(page);
                }
            }
        ]
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: addEditTaskDefinition
            content: AddEditTask{}
        },
        ComponentDefinition {
            id: manageFoldersDefinition
            content: FolderMain {}
        },
        ComponentDefinition {
            id: completedTasksDefinition
            content: CompletedTasks {}
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
