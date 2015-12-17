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
            taskNav.editableContent = true;
        }
        onNewContentAvailableChanged: {
            setFilterAndGrouping({ type: "tasks" }, true);
            taskNav.editableContent = true;
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
            setFilterAndGrouping({ type: "folders" }, false);
            folderNav.editableContent = false;
        }
        onNewContentAvailableChanged: {
            setFilterAndGrouping({ type: "folders" }, false);
            folderNav.editableContent = false;
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
            setFilterAndGrouping({ type: "locations" }, false);
            locationNav.editableContent = false;
        }
        onNewContentAvailableChanged: {
            setFilterAndGrouping({ type: "locations" }, false);
            locationNav.editableContent = false;
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
            setFilterAndGrouping({ type: "contexts" }, false);
            contextNav.editableContent = false;
        }
        onNewContentAvailableChanged: {
            setFilterAndGrouping({ type: "contexts" }, false);
            contextNav.editableContent = false;
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
            setFilterAndGrouping({ type: "goals" }, false);
            goalNav.editableContent = false;
        }
        onNewContentAvailableChanged: {
            setFilterAndGrouping({ type: "goals" }, false);
            goalNav.editableContent = false;
        }
    }
}