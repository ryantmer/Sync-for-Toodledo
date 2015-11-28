import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2

TabbedPane {
    id: mainTabbedPane
    objectName: "mainNavPane"
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                settingsSheetDefinition.createObject().open();
            }
        }
        actions: [
            ActionItem {
                title: "About"
                imageSource: "asset:///images/ic_info.png"
                onTriggered: {
                    aboutSheetDefinition.createObject().open();
                }
            }
        ]
        attachedObjects: [
            ComponentDefinition {
                id: settingsSheetDefinition
                content: Settings {}
            },
            ComponentDefinition {
                id: aboutSheetDefinition
                content: About {}
            }
        ]
    }

    Tab {
        title: "All Tasks"
        ListPage {
            listTitle: "All Tasks"
            backingDataType: "task"
            backingData: app.taskDataModel
        }
    }

    Tab {
        title: "Folders"
        ListPage {
            listTitle: "Folders"
            backingDataType: "folder"
            backingData: app.folderDataModel
        }
    }
}