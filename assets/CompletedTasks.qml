import bb.cascades 1.2

Page {
    accessibility.name: "List of completed tasks"
    titleBar: TitleBar {
        title: "Completed Tasks"
    }
    
    Container {
        accessibility.name: "List of completed tasks container"
        ListView {
            id: completedListView
            accessibility.name: "Main completed tasks list"
            layout: StackListLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
        }
    }
}
