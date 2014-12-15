import bb.cascades 1.2
import bb.data 1.0

Container {
    ListView {
        id: coverListView
        accessibility.name: "Cover task list"
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
                        description: itemContainer.ListItem.view.description(ListItemData.duedate);
                    }
                }
            }
        ]
        
        function description(dueDate) {
            var d = app.unixTimeToDateTime(dueDate);
            var formattedDate = "";
            var day = d.getDate();
            var month = d.getMonth();
            month += 1;
            var year = d.getFullYear();
            
            if (propertyManager.dateFormat == 1) {
                // M/D/Y
                formattedDate = month + "/" + day + "/" + year;
            } else if (propertyManager.dateFormat == 2) {
                // D/M/Y
                formattedDate = day + "/" + month + "/" + year;
            } else if (propertyManager.dateFormat == 3) {
                // Y-M-D
                formattedDate = year + "-" + month + "-" + day;
            } else {
                formattedDate = d.toDateString();
            }
            
            if (dueDate == 0) {
                return "No due date";
            } else if (dueDate <= Math.floor((new Date()).getTime() / 1000)) {
                return "<html><body style=\"color:red\"><b>" + formattedDate + "</b></body></html>";
            } else {
                return formattedDate;
            }
        }
    }
}
