import bb.cascades 1.2

NavigationPane {
    id: mainNavPane
    
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                settingsSheet.open();
            }
        }
        
        actions: [
            ActionItem {
                title: "About"
                onTriggered: {
                    aboutSheet.open();
                }
            }
        ]
    }

    Page {
        signal fetchTasks()
        
        titleBar: TitleBar {
            title: "ToodleDo10"
        }

        Container {
            ListView {
                id: taskListView
                layout: StackListLayout {}
            }
        }

        actions: [
            ActionItem {
                title: "Refresh"
                objectName: "refreshButton"
                ActionBar.placement: ActionBarPlacement.OnBar
                imageSource: "asset:///images/ic_reload.png"
                onTriggered: {
                    fetchTasks();
                    console.log("Refreshed");
                }
            },
            ActionItem {
                title: "Add Task"
                ActionBar.placement: ActionBarPlacement.OnBar
                imageSource: "asset:///images/ic_add.png"
                onTriggered: {
                    addTaskDefinition.createObject().open();
                }
            }
        ]
    }

    attachedObjects: [
        ComponentDefinition {
            id: addTaskDefinition
            source: "asset:///AddTask.qml"
        },
        ComponentDefinition {
            id: editTaskDefinition
            source: "asset:///EditTask.qml"
        },
        Sheet {
            id: settingsSheet
            content: Settings {}
        },
        Sheet {
            id: aboutSheet
            content: About {}
        }
    ]

    onPopTransitionEnded: {
        page.destroy();
    }
}
