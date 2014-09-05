import bb.cascades 1.2
import TaskUtilities 1.0

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
        titleBar: TitleBar {
            title: "ToodleDo10"
        }

        Container {
            ListView {
                id: taskListView
                layout: StackListLayout {
                    headerMode: ListHeaderMode.Sticky
                }
                horizontalAlignment: HorizontalAlignment.Fill
                dataModel: app.dataModel
                
                listItemComponents: [
                    ListItemComponent {
                        Container {
                            CheckBox {
                                text: ListItemData.title
                            }
                        }
                    }
                ]
            }
        }

        actions: [
            ActionItem {
                title: "Refresh"
                objectName: "refreshButton"
                ActionBar.placement: ActionBarPlacement.OnBar
                imageSource: "asset:///images/ic_reload.png"
                onTriggered: {
                    //Actual work is done in C++
                    console.log("Refreshed");
                }
            },
            ActionItem {
                title: "Add Task"
                ActionBar.placement: ActionBarPlacement.OnBar
                imageSource: "asset:///images/ic_add.png"
                onTriggered: {
                    addTaskSheet.open();
                }
            }
        ]
    }

    attachedObjects: [
        Sheet {
            id: addTaskSheet
            content: AddTask {}
        },
        Sheet {
            id: editTaskSheet
            content: EditTask {}
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
