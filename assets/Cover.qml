import bb.cascades 1.2

Container {
    accessibility.name: "Cover container"
    layout: StackLayout {}
    
    topPadding: 10
    bottomPadding: 10
    leftPadding: 10
    rightPadding: 10
    
    Label {
        id: taskTitle
        objectName: "taskTitle"
    }
    Label {
        id: taskDue
        objectName: "taskDue"
    }
}
