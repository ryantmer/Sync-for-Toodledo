import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2

Page {
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
            onTriggered: {
                var task = dataModel.data(indexPath);
                var page = addEditTaskDefinition.createObject();
                page.data = task;
                page.edit = true;
                page.setup();
                mainNavPane.push(page);
            }
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
