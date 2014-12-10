import bb.cascades 1.2
import bb.data 1.0

Container {
    ListView {
        id: coverListView
        layout: StackListLayout {}
        horizontalAlignment: HorizontalAlignment.Fill
        
        dataModel: app.taskDataModel
        
        listItemComponents: [
            ListItemComponent {
                type: "item"
                Container {
                    id: itemContainer
                    StandardListItem {
                        title: ListItemData.title
                        description: itemContainer.ListItem.view.dueDateString(ListItemData.duedate);
                    }
                }
            }
        ]
        
        function dueDateString(dueDate) {
            if (dueDate == 0) {
                return "No due date";
            } else if (dueDate <= Math.floor((new Date()).getTime() / 1000)) {
                return "<html><body style=\"color:red\"><b>" +
                app.unixTimeToDateTime(dueDate).toDateString() +
                "</b></body></html>";
            } else {
                return app.unixTimeToDateTime(dueDate).toDateString();
            }
        }
    }
}
