/*
 * ListView page which does not allow for adding/deleting. When an item is 
 * selected, it filters the underlying data model based on the selection.
 */

import bb.cascades 1.4
import bb.data 1.0
import bb.system 1.2
import DataModelUtil 1.0

NavigationPane {
    id: listNavPane
    property string listTitle
    
    function setFilter(newFilter) {
        listView.dataModel.filter = newFilter;
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
                    listView.dataModel.refresh();
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
                    console.log("Open " + indexPath);
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
                            }
                        }
                    }
                ]
            }
        }
    }
}