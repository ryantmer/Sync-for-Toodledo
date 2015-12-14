/*
 * ListView page which does not allow for adding/deleting. When an item is
 * selected, it filters the underlying data model based on the selection.
 */

import bb.cascades 1.4
import bb.data 1.0
import bb.system 1.2

NavigationPane {
    id: listNavPaneReadOnly
    property string listTitle
    property variant myFilter
    property bool myGrouping

    function setup(filter, grouping) {
        app.data.filter = filter;
        app.data.itemsGrouped = grouping;

        myFilter = filter;
        myGrouping = grouping;
    }

    onPopTransitionEnded: {
        app.data.filter = myFilter;
        app.data.itemsGrouped = myGrouping;
        listView.dataModel = app.data;
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
            }
        ]

        Container {
            Label {
                text: "Hm, didn't find anything..."
                visible: listView.dataModel != null && listView.dataModel.empty
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

                    var page = pageDef.createObject();
                    page.listTitle = data.name;

                    var filter = {
                        type: "tasks"
                    };
                    if (dataModel.filter.type == "folders") {
                        filter.folder = data.id;
                    } else if (dataModel.filter.type == "locations") {
                        filter.location = data.id;
                    } else if (dataModel.filter.type == "contexts") {
                        filter.context = data.id;
                    } else if (dataModel.filter.type == "goals") {
                        filter.goal = data.id;
                    }
                    
                    // This prevents some weird visuals - better for the page 
                    // to go blank than to briefly show the tasks in the given 
                    // FCGL before the new page opens fully.
                    listView.dataModel = null;
                    
                    app.data.filter = filter;
                    app.data.itemsGrouped = true;

                    listNavPaneReadOnly.push(page);
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
                attachedObjects: [
                    ComponentDefinition {
                        id: pageDef
                        content: ListPageNoNav {
                        }
                    }
                ]
            }
        }
    }
}