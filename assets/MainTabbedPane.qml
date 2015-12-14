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
    
    function setFilterAndGrouping(filter, grouping) {
        app.data.filter = filter;
        app.data.itemsGrouped = grouping;
    }

    Tab {
        title: "All Tasks"
        imageSource: "asset:///images/checkmark.png"
        Navigation {
            id: taskNav
            listTitle: "All Tasks"
        }
        onTriggered: {
            setFilterAndGrouping({ type: "tasks" }, true);
        }
        onNewContentAvailableChanged: {
            setFilterAndGrouping({ type: "tasks" }, true);
        }
    }

    Tab {
        title: "Folders"
        imageSource: "asset:///images/ic_folders.png"
        Navigation {
            id: folderNav
            listTitle: "Folders"
        }
        onTriggered: {
            setFilterAndGrouping({ type: "folders" }, true);
        }
        onNewContentAvailableChanged: {
            setFilterAndGrouping({ type: "folders" }, true);
        }
    }

    Tab {
        title: "Locations"
        imageSource: "asset:///images/ic_locations.png"
        Navigation {
            id: locationNav
            listTitle: "Locations"
        }
        onTriggered: {
            setFilterAndGrouping({ type: "locations" }, true);
        }
        onNewContentAvailableChanged: {
            setFilterAndGrouping({ type: "locations" }, true);
        }
    }

    Tab {
        title: "Contexts"
        imageSource: "asset:///images/ic_contexts.png"
        Navigation {
            id: contextNav
            listTitle: "Contexts"
        }
        onTriggered: {
            setFilterAndGrouping({ type: "contexts" }, true);
        }
        onNewContentAvailableChanged: {
            setFilterAndGrouping({ type: "contexts" }, true);
        }
    }

    Tab {
        title: "Goals"
        imageSource: "asset:///images/ic_goals.png"
        Navigation {
            id: goalNav
            listTitle: "Goals"
        }
        onTriggered: {
            setFilterAndGrouping({ type: "goals" }, true);
        }
        onNewContentAvailableChanged: {
            setFilterAndGrouping({ type: "goals" }, true);
        }
    }
}