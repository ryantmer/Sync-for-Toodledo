import bb.cascades 1.2
import bb.data 1.0
import bb.system 1.2

TabbedPane {
    id: mainTabbedPane
    objectName: "mainTabbedPane"
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
            id: taskListPage
            listTitle: "All Tasks"
        }
        onTriggered: {
            taskListPage.setFilter({
                type: "tasks"
            });
            taskListPage.setGrouping(true);
        }
        onNewContentAvailableChanged: {
            taskListPage.setFilter({
                    type: "tasks"
            });
            taskListPage.setGrouping(true);
        }
    }

    Tab {
        title: "Folders"
        imageSource: "asset:///images/ic_folders.png"
        ListPageReadOnly {
            id: folderListPage
            listTitle: "Folders"
        }
        onTriggered: {
            folderListPage.setFilter({
                type: "folders"
            });
            taskListPage.setGrouping(false);
        }
        onNewContentAvailableChanged: {
            taskListPage.setFilter({
                    type: "folders"
            });
            taskListPage.setGrouping(true);
        }
    }

    Tab {
        title: "Locations"
        imageSource: "asset:///images/ic_locations.png"
        ListPageReadOnly {
            id: locationListPage
            listTitle: "Locations"
        }
        onTriggered: {
            locationListPage.setFilter({
                type: "locations"
            });
            taskListPage.setGrouping(false);
        }
        onNewContentAvailableChanged: {
            taskListPage.setFilter({
                    type: "locations"
            });
            taskListPage.setGrouping(true);
        }
    }

    Tab {
        title: "Contexts"
        imageSource: "asset:///images/ic_contexts.png"
        ListPageReadOnly {
            id: contextListPage
            listTitle: "Contexts"
        }
        onTriggered: {
            contextListPage.setFilter({
                type: "contexts"
            });
            taskListPage.setGrouping(false);
        }
        onNewContentAvailableChanged: {
            taskListPage.setFilter({
                    type: "contexts"
            });
            taskListPage.setGrouping(true);
        }
    }

    Tab {
        title: "Goals"
        imageSource: "asset:///images/ic_goals.png"
        ListPageReadOnly {
            id: goalListPage
            listTitle: "Goals"
        }
        onTriggered: {
            goalListPage.setFilter({
                type: "goals"
            });
            taskListPage.setGrouping(false);
        }
        onNewContentAvailableChanged: {
            taskListPage.setFilter({
                    type: "goals"
            });
            taskListPage.setGrouping(true);
        }
    }
}