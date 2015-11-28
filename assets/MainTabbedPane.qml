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
        ListPage {
            listTitle: "All Tasks"
            backingDataType: "task"
            backingData: app.tasks
        }
    }
    
    Tab {
        title: "Hotlist"
        ListPage {
            listTitle: "Hotlist"
            backingDataType: "task"
            backingData: app.hotlist
        }
    }
    
    Tab {
        title: "Recently Completed"
        ListPage {
            listTitle: "Recently Completed"
            backingDataType: "task"
            backingData: app.completedTasks
        }
    }
    
    Tab {
        title: "Folders"
        ListPage {
            listTitle: "Folders"
            backingDataType: "folder"
            backingData: app.folders
        }
    }
    
    Tab {
        title: "Locations"
        ListPage {
            listTitle: "Locations"
            backingDataType: "location"
            backingData: app.locations
        }
    }
    
    Tab {
        title: "Contexts"
        ListPage {
            listTitle: "Contexts"
            backingDataType: "context"
            backingData: app.contexts
        }
    }
    
    Tab {
        title: "Goals"
        ListPage {
            listTitle: "Goals"
            backingDataType: "goal"
            backingData: app.goals
        }
    }
}