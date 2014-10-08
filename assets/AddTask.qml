import bb.cascades 1.2

Sheet {
    id: addTaskSheet
    
    onClosed: {
        destroy();
    }
    
    Page {
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
            layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
            topPadding: 10
            leftPadding: 10
            rightPadding: 10
            bottomPadding: 10
            
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