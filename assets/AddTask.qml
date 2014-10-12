import bb.cascades 1.2

Sheet {
    id: addTaskSheet
    accessibility.name: "Add task sheet"
    
    onClosed: {
        destroy();
    }
    
    Page {
        accessibility.name: "Add task page"
        
        titleBar: TitleBar {
            title: "Add Task"
            visibility: ChromeVisibility.Visible
            dismissAction: ActionItem {
                title: "Cancel"
                onTriggered: {
                    addTaskSheet.close();
                }
            }
        
            acceptAction: ActionItem {
                title: "Add"
                property variant taskData
                
                onTriggered: {
                    var d;
                    var n;
                    
                    if (!taskName.text) {
                        app.showToast("Task name required");
                        return;
                    }
                    
                    if (noDueDate.checked) {
                        d = 0;
                    } else {
                        d = app.dateTimeToUnixTime(taskDueDate.value);
                    }
                    
                    taskData = {"title": taskName.text,
                                "duedate": d,
                                "note": taskNote.text};
                    
                    app.addTask(taskData);
                    addTaskSheet.close();
                }
            }
        }
        
        Container {
            accessibility.name: "Add task page container"
            layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
            topPadding: 30
            leftPadding: 30
            rightPadding: 30
            bottomPadding: 30
            
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
                    taskDueDate.enabled = ! noDueDate.checked;
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