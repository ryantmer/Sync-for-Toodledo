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
            app.data.filter = {
                type: "tasks"
            }
            app.data.itemsGrouped = true;
        }
        onNewContentAvailableChanged: {
            app.data.filter = {
                type: "tasks"
            }
            app.data.itemsGrouped = true;
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
            folderListPage.setup({ type: "folders" }, false);
        }
        onNewContentAvailableChanged: {
            folderListPage.setup({ type: "folders" }, false);
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
            locationListPage.setup({ type: "locations" }, false);
        }
        onNewContentAvailableChanged: {
            locationListPage.setup({ type: "locations" }, false);
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
            contextListPage.setup({ type: "contexts" }, false);
        }
        onNewContentAvailableChanged: {
            contextListPage.setup({ type: "contexts" }, false);
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
            goalListPage.setup({ type: "goals" }, false);
        }
        onNewContentAvailableChanged: {
            goalListPage.setup({ type: "goals" }, false);
        }
    }
}