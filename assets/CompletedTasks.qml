import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2

Page {
    titleBar: TitleBar {
        title: "Sync for Toodledo - Completed Tasks"
    }
    
    Container {
        ListView {
            id: listView
            accessibility.name: "Completed tasks list"
            layout: StackListLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            
            dataModel: app.completedTaskDataModel
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    Container {
                        id: itemContainer
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        leftPadding: 10.0
                        
                        CheckBox {
                            accessibility.name: "Completed task checkbox"
                            //This should be ListItemData.completed... but doing this calls
                            //onCheckedChanged for some reason, which edits the task
                            //and fucks everything up w.r.t modified/completed timestamps.
                            //Whatever. They're all going to be checked anyway.
                            checked: true;
                            verticalAlignment: VerticalAlignment.Center
                            
                            onCheckedChanged: {
                                if (checked) {
                                    var oldData = {"id": parseInt(ListItemData.id),
                                        "completed": ListItemData.completed};
                                    var newData = {"id": parseInt(ListItemData.id),
                                        "completed": Math.floor((new Date()).getTime() / 1000)};
                                    app.completedTaskDataModel.edit(oldData, newData);
                                } else {
                                    var oldData = {"id": parseInt(ListItemData.id),
                                        "completed": ListItemData.completed};
                                    var newData = {"id": parseInt(ListItemData.id),
                                        "completed": 0};
                                    app.completedTaskDataModel.edit(oldData, newData);
                                }
                            }
                        }
                        StandardListItem {
                            title: ListItemData.title
                            description: itemContainer.ListItem.view.completedString(ListItemData.completed);
                            textFormat: TextFormat.Auto
                        }
                    }
                }
            ]
            
            function completedString(completed) {
                return "Completed on " + app.unixTimeToDateTime(completed).toDateString();
            }
        }
    }
    
    actions: [
        ActionItem {
            title: "Refresh"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_reload.png"
            onTriggered: {
                app.completedTaskDataModel.refresh();
            }
        }
    ]
}