/*
 * ListView page which has an Add button, Delete context action, and opens an 
 * edit page for an item when it is selected.
 */

import bb.cascades 1.4
import bb.data 1.0
import bb.system 1.2

NavigationPane {
    id: listNavPane
    property string listTitle
    property string backingDataType
    property variant backingData

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
                    backingData.refresh();
                }
            },
            ActionItem {
                title: "Add"
                ActionBar.placement: ActionBarPlacement.OnBar
                imageSource: "asset:///images/ic_add.png"
                onTriggered: {
                    var page = addPageDefinition.createObject();
                    listNavPane.push(page);
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
                property alias backingDataType: listNavPane.backingDataType
                layout: StackListLayout {
                }
                horizontalAlignment: HorizontalAlignment.Fill
                dataModel: backingData
                onTriggered: {
                    var item = dataModel.data(indexPath);
                    var page = editPageDefinition.createObject();
                    page.backingDataType = backingDataType;
                    page.data = item;
                    page.setup();
                    listNavPane.push(page);
                }
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        Container {
                            id: itemContainer
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            leftPadding: 10.0

                            StandardListItem {
                                title: ListItemData.title
                                description: ListItemData.description
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
                                        title: "Delete " + itemContainer.ListItem.view.backingDataType
                                        body: "Are you sure you want to delete this " + itemContainer.ListItem.view.backingDataType + "?"
                                        confirmButton.label: "Do it!"
                                        confirmButton.enabled: true
                                        cancelButton.label: "Cancel"
                                        cancelButton.enabled: true
                                        onFinished: {
                                            if (result == SystemUiResult.ConfirmButtonSelection) {
                                                backingData.remove(ListItemData.id);
                                            }
                                        }
                                    }
                                ]
                            }
                        }
                    }
                ]
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