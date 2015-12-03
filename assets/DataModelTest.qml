import bb.cascades 1.4
import DataModelUtil 1.0

Page {
    Container {
        Button {
            text: "Filter to just folders"
            onClicked: {
                folderDataModel.filter = "Folder"
            }
        }
        Button {
            text: "Reset"
            onClicked: {
                folderDataModel.filter = "";
            }
        }
        ListView {
            id: statesListView
            dataModel: FilterDataModel {
                id: folderDataModel
                filterOn: "type"
            }
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        title: ListItemData.name != null ? ListItemData.name : ListItemData.title
                        description: ListItemData.type
                    }
                }
            ]
        }
    }
}