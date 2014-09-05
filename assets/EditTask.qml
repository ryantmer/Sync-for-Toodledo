import bb.cascades 1.2

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
            objectName: "editTaskButton"
            signal triggered(variant data)
            property variant taskData
            onTriggered: {
                busyIndicator.running = true;
                taskData = [{"completed":taskCompleted.checked},
                            {"title":taskName.text},
                            {"duedate":taskDueDate.value},
                            {"notes":taskNotes.text}]
                triggered(taskData);
                console.log("Task Edited");
                busyIndicator.running = false;
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
        }
        TextField {
            id: taskName
            hintText: "Task Name"
            horizontalAlignment: HorizontalAlignment.Fill
            topMargin: 10.0
            leftMargin: 10.0
            rightMargin: 10.0
            bottomMargin: 10.0
        }
        DateTimePicker {
            id: taskDueDate
            horizontalAlignment: HorizontalAlignment.Fill
            mode: DateTimePickerMode.Date
            expanded: true
            title: "Due Date"
            topMargin: 10.0
            leftMargin: 10.0
            rightMargin: 10.0
            bottomMargin: 10.0
        }
        TextArea {
            id: taskNotes
            hintText: "Detailed notes about task"
            horizontalAlignment: HorizontalAlignment.Fill
            topMargin: 10.0
            leftMargin: 10.0
            rightMargin: 10.0
            bottomMargin: 10.0
        }
    }
        
    attachedObjects: [
        ActivityIndicator {
            id: busyIndicator
            running: false
        }
    ]
}