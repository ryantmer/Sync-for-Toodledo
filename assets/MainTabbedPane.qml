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
                content: Settings {
                }
            },
            ComponentDefinition {
                id: aboutSheetDefinition
                content: About {
                }
            }
        ]
    }

    Tab {
        title: "All Tasks"
        imageSource: "asset:///images/checkmark.png"
        ListPageEditable {
            listTitle: "All Tasks"
            backingDataType: "task"
            backingData: app.tasks
        }
    }
    
    Tab {
        title: "Data Model test"
        DataModelTest {
        }
    }

//    Not yet implemented on the backend
//    Tab {
//        title: "Hotlist"
//        ListPageEditable {
//            listTitle: "Hotlist"
//            backingDataType: "task"
//            backingData: app.hotlist
//        }
//    }
    
//    Not yet implemented on the backend
//    Tab {
//        title: "Recently Completed"
//        ListPageEditable {
//            listTitle: "Recently Completed"
//            backingDataType: "task"
//            backingData: app.completedTasks
//        }
//    }

    Tab {
        title: "Folders"
        imageSource: "asset:///images/ic_folders.png"
        ListPageReadOnly {
            listTitle: "Folders"
            backingDataType: "folder"
            backingData: app.folders
        }
    }

    Tab {
        title: "Locations"
        imageSource: "asset:///images/ic_locations.png"
        ListPageReadOnly {
            listTitle: "Locations"
            backingDataType: "location"
            backingData: app.locations
        }
    }

    Tab {
        title: "Contexts"
        imageSource: "asset:///images/ic_contexts.png"
        ListPageReadOnly {
            listTitle: "Contexts"
            backingDataType: "context"
            backingData: app.contexts
        }
    }

    Tab {
        title: "Goals"
        imageSource: "asset:///images/ic_goals.png"
        ListPageReadOnly {
            listTitle: "Goals"
            backingDataType: "goal"
            backingData: app.goals
        }
    }
}