import bb.cascades 1.4
import bb.data 1.0
import bb.system 1.2

Page {
    property string listTitle
    property bool editableContent
    
    titleBar: TitleBar {
        title: "Sync for Toodledo - " + listTitle
    }
    actions: [
        ActionItem {
            title: "Refresh"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_reload.png"
            onTriggered: {
                listView.dataModel.refresh(listView.dataModel.filter.type);
            }
        },
        ActionItem {
            id: addButton
            title: "Add"
            enabled: editableContent
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_add.png"
            onTriggered: {
                var page = addPageDefinition.createObject();
                page.setup();
                navigationPane.push(page);
            }
        }
    ]
    attachedObjects: [
        ComponentDefinition {
            id: addPageDefinition
            content: AddPage {
            }
        },
        ComponentDefinition {
            id: listPageChildDefinition
            content: ListPageChild {
            }
        }
    ]
    
    Container {
        Label {
            text: "Hm, didn't find anything..."
            visible: listView.dataModel.empty
            horizontalAlignment: HorizontalAlignment.Center
        }
        ListView {
            id: listView
            layout: StackListLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            dataModel: app.data
            onTriggered: {
                var item = dataModel.data(indexPath);
                if (dataModel.filter.type == "tasks") {
                    var page = editPageDefinition.createObject();
                    page.data = item;
                    page.setup();
                    navigationPane.push(page);
                    return;
                }
                
                var page = listPageChildDefinition.createObject();
                page.listTitle = item.name;
                page.previousFilter = {
                    type: dataModel.filter.type
                };
                var filter = {
                    type: "tasks"
                };
                
                if (dataModel.filter.type == "folders") {
                    filter.folder = item.id;
                } else if (dataModel.filter.type == "locations") {
                    filter.location = item.id;
                } else if (dataModel.filter.type == "contexts") {
                    filter.context = item.id;
                } else if (dataModel.filter.type == "goals") {
                    filter.goal = item.id;
                }
                
                app.data.filter = filter;
                navigationPane.push(page);
            }
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    Header {
                        title: ListItemData
                    }
                },
                ListItemComponent {
                    type: "item"
                    Container {
                        id: itemContainer
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        leftPadding: 10.0
                        
                        CheckBox {
                            checked: ListItemData.completed
                            verticalAlignment: VerticalAlignment.Center
                            visible: ListItemData.name == null // super hax - this will only be enabled for tasks now
                            onCheckedChanged: {
                                var data = {
                                    id: parseInt(ListItemData.id),
                                    completed: checked ? Math.floor((new Date()).getTime() / 1000) : 0
                                };
                                itemContainer.ListItem.view.dataModel.editItem("tasks", data);
                            }
                        }
                        
                        StandardListItem {
                            title: ListItemData.title
                            description: itemContainer.ListItem.view.parseNote(ListItemData.note)
                            textFormat: TextFormat.Auto
                            contextActions: [
                                ActionSet {
                                    DeleteActionItem {
                                        enabled: ListItemData.name == null
                                        onTriggered: {
                                            deleteConfirmDialog.show();
                                        }
                                    }
                                }
                            ]
                            attachedObjects: [
                                SystemDialog {
                                    id: deleteConfirmDialog
                                    title: "Delete " + itemContainer.ListItem.view.dataModel.filter
                                    body: "Are you sure?"
                                    confirmButton.label: "Do it!"
                                    confirmButton.enabled: true
                                    cancelButton.label: "Cancel"
                                    cancelButton.enabled: true
                                    onFinished: {
                                        if (result == SystemUiResult.ConfirmButtonSelection) {
                                            app.data.removeItem(itemContainer.ListItem.view.dataModel.filter.type, {
                                                    id: ListItemData.id
                                            });
                                        }
                                    }
                                }
                            ]
                        }
                    }
                }
            ]
            
            function parseNote(note) {
                if (note && note.indexOf("\n") > -1) {
                    //Note is multi-line, take first line as description
                    return note.substring(0, note.indexOf("\n"));
                } else {
                    //Note is a single line
                    return note;
                }
            }
            
            function parseDate(dueDate) {
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
            
            attachedObjects: [
                ComponentDefinition {
                    id: editPageDefinition
                    content: EditPage {
                    }
                }
            ]
        }
    }
}