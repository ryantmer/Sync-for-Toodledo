import bb.cascades 1.2

Sheet {
    id: editTaskSheet
    
    onClosed: {
        destroy();
    }
    
    property alias taskId: taskIdField.text
    property alias completed: taskCompleted.checked
    property alias title: taskName.text
    property alias duedate: taskDueDate.value
    property alias dueDateEnabled: taskDueDate.enabled
    property alias noDueDate: noDueDate.checked
    property alias note: taskNote.text
    property variant oldData;
    
    Page {
        titleBar: TitleBar {
            title: "Edit Task"
            visibility: ChromeVisibility.Visible
            dismissAction: ActionItem {
                title: "Cancel"
                onTriggered: {
                    editTaskSheet.close();
                }
            }
            
            acceptAction: ActionItem {
                title: "Save"
                property variant taskData
                
                onTriggered: {
                    var c;
                    var d;
                    
                    if (!taskName.text) {
                        app.showToast("Task name required");
                        return;
                    }
                    
                    if (taskCompleted.checked) {
                        c = Math.floor((new Date()).getTime() / 1000);
                    } else {
                        c = 0;
                    }
                    if (noDueDate.checked) {
                        d = 0;
                    } else {
                        d = app.dateTimeToUnixTime(taskDueDate.value);
                    }
                    
                    taskData = {"id": parseInt(taskIdField.text),
                                "completed": c,
                                "title": taskName.text,
                                "duedate": d,
                                "note": taskNote.text};
                    
                    app.editTask(oldData, taskData);
                    editTaskSheet.close();
                }
            }
        }
            
        Container {
            layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
            topPadding: 30
            leftPadding: 30
            rightPadding: 30
            bottomPadding: 30
                
            CheckBox {
                id: taskCompleted
                text: "Completed"
                topMargin: 40
                bottomMargin: 40
            }
            TextField {
                id: taskIdField
                visible: false
                topMargin: 40
                bottomMargin: 40
                accessibility.name: "Task ID Field"
                accessibility.description: "Hidden field containing task's ID number"
            }
            TextField {
                id: taskName
                hintText: "Task Name"
                horizontalAlignment: HorizontalAlignment.Fill
                topMargin: 40
                bottomMargin: 40
            }
            CheckBox {
                id: noDueDate
                text: "No Due Date"
                topMargin: 40
                bottomMargin: 40
                onCheckedChanged: {
                    taskDueDate.enabled = !noDueDate.checked;
                }
            }
            DateTimePicker {
                id: taskDueDate
                horizontalAlignment: HorizontalAlignment.Fill
                mode: DateTimePickerMode.Date
                expanded: false
                title: "Due Date"
                topMargin: 40
                bottomMargin: 40
            }
            TextArea {
                id: taskNote
                hintText: "Detailed notes about task"
                horizontalAlignment: HorizontalAlignment.Fill
                topMargin: 40
                bottomMargin: 40
            }
        }
    }
}