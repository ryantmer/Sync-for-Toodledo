/*
 * ListView page which has an Add button, Delete context action, and opens an
 * edit page for an item when it is selected.
 */

import bb.cascades 1.4
import bb.data 1.0
import bb.system 1.2

NavigationPane {
    id: listNavPaneEditable
    property string listTitle
    
    onPopTransitionEnded: {
        page.destroy();
    }

    Page {
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
                title: "Add"
                ActionBar.placement: ActionBarPlacement.OnBar
                imageSource: "asset:///images/ic_add.png"
                onTriggered: {
                    var page = addPageDefinition.createObject();
                    listNavPaneEditable.push(page);
                }
            }
        ]
        attachedObjects: [
            ComponentDefinition {
                id: addPageDefinition
                content: AddPage {
                }
            }
        ]

        Container {
            Label {
                text: "Hm, didn't find anything... Add one below!"
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
                    var data = dataModel.data(indexPath);
                    var page = editPageDefinition.createObject();
                    page.data = data;
                    page.setup();
                    listNavPaneEditable.push(page);
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
                                visible: itemContainer.ListItem.view.dataModel.filter == "tasks"
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
                                                app.data.removeItem(itemContainer.ListItem.view.dataModel.filter, {
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
                    if ('string' == typeof note && note.indexOf("\n") > -1) {
                        //Note is multi-line, take first line as description
                        return note.substring(0, note.indexOf("\n"));
                    } else {
                        //Note is a single line, or undefined
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

                function itemType(data, indexPath) {
                    if (indexPath.length == 1) {
                        data = {
                            stuff: "things"
                        };
                        return "header";
                    }
                    return "item";
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
}