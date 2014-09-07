import bb.cascades 1.2

NavigationPane {
    id: mainNavPane
    
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
                            Label {
                                text: ListItemData.title
                            }
                        }
                    }
                ]
                
                onTriggered: {
                    var p = editTaskSheetDefinition.createObject();
                    var selectedTask = dataModel.data(indexPath);
                    p.completed = selectedTask.completed;
                    p.duedate = app.unixTimeToDateTime(selectedTask.duedate);
                    p.title = selectedTask.title;
                    p.open();
                }
            }
        }

        actions: [
            ActionItem {
                title: "Refresh"
                ActionBar.placement: ActionBarPlacement.OnBar
                imageSource: "asset:///images/ic_reload.png"
                onTriggered: {
                    app.updateDataModel();
                    console.log("Refreshed");
                }
            },
            ActionItem {
                title: "Add Task"
                ActionBar.placement: ActionBarPlacement.OnBar
                imageSource: "asset:///images/ic_add.png"
                onTriggered: {
                    var p = addTaskSheetDefinition.createObject();
                    p.open();
                }
            }
        ]
    }

    attachedObjects: [
        ComponentDefinition {
            id: addTaskSheetDefinition
            content: AddTask {}
        },
        ComponentDefinition {
            id: editTaskSheetDefinition
            content: EditTask {}
        },
        ComponentDefinition {
            id: settingsSheetDefinition
            content: Settings {}
        },
        ComponentDefinition {
            id: aboutSheetDefinition
            content: About {}
        }
    ]

    onPopTransitionEnded: {
        page.destroy();
    }
}
