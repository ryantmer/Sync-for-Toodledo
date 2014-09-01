import bb.cascades 1.2

Sheet {
    id: addTaskSheet
    Page {
        id: addTaskPage
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
                onTriggered: {
                    busyIndicator.running = true;
                    //TODO: Add task to data model here
                    console.log("Task Added");
                    busyIndicator.running = false;
                    addTaskSheet.close();
                }
            }
        }
    
        Container {
            layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
            topPadding: 10.0
            leftPadding: 10.0
            rightPadding: 10.0
            bottomPadding: 10.0
            TextField {
                hintText: "Task Name"
                horizontalAlignment: HorizontalAlignment.Fill
                topMargin: 10.0
                leftMargin: 10.0
                rightMargin: 10.0
                bottomMargin: 10.0
            }
            DateTimePicker {
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
}