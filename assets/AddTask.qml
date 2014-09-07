import bb.cascades 1.2

Sheet {
    id: addTaskSheet
    
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
                property var taskData
                
                onTriggered: {
                    taskData = [{"completed": taskCompleted.checked},
                                {"title": taskName.text},
                                {"duedate": app.dateTimeToUnixTime(taskDueDate.value)},
                                {"notes": taskNotes.text}]
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
            
            CheckBox {
                id: taskCompleted
                text: "Completed"
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
            DateTimePicker {
                id: taskDueDate
                horizontalAlignment: HorizontalAlignment.Fill
                mode: DateTimePickerMode.Date
                expanded: true
                title: "Due Date"
                topMargin: 20
                leftMargin: 20
                rightMargin: 20
                bottomMargin: 20
            }
    
            TextArea {
                id: taskNotes
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