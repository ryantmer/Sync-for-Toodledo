import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2

NavigationPane {
    id: mainNavPane
    objectName: "mainNavPane"
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                var page = settingsSheetDefinition.createObject()
                page.setup();
                page.open();
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
    attachedObjects: [
        ComponentDefinition {
            id: addEditTaskDefinition
            content: AddEditTask{}
        },
        ComponentDefinition {
            id: completedTasksDefinition
            content: ListCompletedTasks {}
        },
        ComponentDefinition {
            id: foldersDefinition
            content: ListFolders {}
        },
        ComponentDefinition {
            id: contextsDefinition
            content: ListContexts{}
        },
        ComponentDefinition {
            id: goalsDefinition
            content: ListGoals{}
        },
        ComponentDefinition {
            id: locationsDefinition
            content: ListLocations{}
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
    
    Page {
        id: tasksPage
        objectName: "tasksPage"
        titleBar: TitleBar {
            title: "Sync for Toodledo - All Tasks"
        }
        actions: [
            ActionItem {
                title: "Refresh"
                ActionBar.placement: ActionBarPlacement.OnBar
                imageSource: "asset:///images/ic_reload.png"
                onTriggered: {
                    app.taskDataModel.refresh();
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
                title: "Completed Tasks"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/ic_completed.png"
                onTriggered: {
                    var page = completedTasksDefinition.createObject();
                    mainNavPane.push(page);
                }
            },
            ActionItem {
                title: "Manage Folders"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/ic_folders.png"
                onTriggered: {
                    var page = foldersDefinition.createObject();
                    mainNavPane.push(page);
                }
            },
            ActionItem {
                title: "Manage Contexts"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/ic_contexts.png"
                onTriggered: {
                    var page = contextsDefinition.createObject();
                    mainNavPane.push(page);
                }
            },
            ActionItem {
                title: "Manage Goals"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/ic_goals.png"
                onTriggered: {
                    var page = goalsDefinition.createObject();
                    mainNavPane.push(page);
                }
            },
            ActionItem {
                title: "Manage Locations"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/ic_locations.png"
                onTriggered: {
                    var page = locationsDefinition.createObject();
                    mainNavPane.push(page);
                }
            }
        ]
        
        Container {
            Label {
                text: "You don't have any tasks!"
                visible: listView.dataModel.empty
                horizontalAlignment: HorizontalAlignment.Center
            }
            ListView {
                id: listView
                layout: StackListLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                dataModel: app.taskDataModel
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        Container {
                            id: itemContainer
                            layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
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
                                        app.taskDataModel.edit(oldData, newData);
                                    } else {
                                        var oldData = {"id": parseInt(ListItemData.id),
                                            "completed": ListItemData.completed};
                                        var newData = {"id": parseInt(ListItemData.id),
                                            "completed": 0};
                                        app.taskDataModel.edit(oldData, newData);
                                    }
                                }
                            }
                            StandardListItem {
                                title: ListItemData.title
                                status: itemContainer.ListItem.view.status(ListItemData.duedate);
                                description: itemContainer.ListItem.view.description(ListItemData.note);
                                textFormat: TextFormat.Auto
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
                                        title: "Delete Task"
                                        body: "Are you sure you want to delete this task?"
                                        confirmButton.label: "Delete"
                                        confirmButton.enabled: true
                                        cancelButton.label: "Cancel"
                                        cancelButton.enabled: true
                                        onFinished: {
                                            if (result == SystemUiResult.ConfirmButtonSelection) {
                                                var taskData = {"id": ListItemData.id}
                                                app.taskDataModel.remove(taskData);
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
                function status(dueDate) {
                    var d = app.unixTimeToDateTime(dueDate);
                    var formattedDate = "";
                    var day = d.getDate();
                    var month = d.getMonth();
                    month += 1;
                    var year = d.getFullYear();
                    
                    if (propertyManager.dateFormat == 1) {
                        // M/D/Y
                        formattedDate = month + "/" + day + "/" + year;
                    } else if (propertyManager.dateFormat == 2) {
                        // D/M/Y
                        formattedDate = day + "/" + month + "/" + year;
                    } else if (propertyManager.dateFormat == 3) {
                        // Y-M-D
                        formattedDate = year + "-" + month + "-" + day;
                    } else {
                        formattedDate = d.toDateString();
                    }
                    
                    if (dueDate == 0) {
                        return "No due date";
                    } else if (dueDate <= Math.floor((new Date()).getTime() / 1000)) {
                        return "<html><body style=\"color:red\"><b>" + formattedDate + "</b></body></html>";
                    } else {
                        return formattedDate;
                    }
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
    }
}
