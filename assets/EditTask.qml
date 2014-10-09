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
                    
                    app.editTask(taskData);
                    editTaskSheet.close();
                }
            }
        }
            
        Container {
            layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
            topPadding: 10.0
            leftPadding: 10.0
            rightPadding: 10.0
            bottomPadding: 10.0
                
            CheckBox {
                id: taskCompleted
                text: "Completed"
                topMargin: 20
                leftMargin: 20
                rightMargin: 20
                bottomMargin: 20
            }
            TextField {
                id: taskIdField
                visible: false
                topMargin: 20
                leftMargin: 20
                rightMargin: 20
                bottomMargin: 20
            }
            TextField {
                id: taskName
                hintText: "Task Name"
                horizontalAlignment: HorizontalAlignment.Fill
                topMargin: 20
                leftMargin: 20
                rightMargin: 20
                bottomMargin: 20
            }
            CheckBox {
                id: noDueDate
                text: "No Due Date"
                topMargin: 20
                leftMargin: 20
                rightMargin: 20
                bottomMargin: 20
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
                topMargin: 20
                leftMargin: 20
                rightMargin: 20
                bottomMargin: 20
            }
            TextArea {
                id: taskNote
                hintText: "Detailed notes about task"
                horizontalAlignment: HorizontalAlignment.Fill
                topMargin: 20
                leftMargin: 20
                rightMargin: 20
                bottomMargin: 20
            }
        }
    }
}