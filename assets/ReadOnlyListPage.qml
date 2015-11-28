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
                property alias backingDataType: listNavPane.backingDataType
                layout: StackListLayout {
                }
                horizontalAlignment: HorizontalAlignment.Fill
                dataModel: backingData
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