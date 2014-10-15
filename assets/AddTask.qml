import bb.cascades 1.2

Page {
    id: addTaskPage
    accessibility.name: "Add task page"
    
    function populateFolderDropDown() {
        var folders = app.getFolderList();
        
        for (var x = 0; x < folders.length; x++) {
            var opt = option.createObject();
            opt.text = folders[x].name;
            opt.value = folders[x].id;
            folderDropDown.add(opt);
        }
    }
    
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: "Cancel"
            onTriggered: {
                mainNavPane.pop();
            }
        }
    }
    
    actions: [
        ActionItem {
            id: saveButton
            title: "Add"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_save.png"
            onTriggered: {
                var d;
                var n;
                
                if (!taskName.text) {
                    titleRequired.visible = true;
                    return;
                }
                
                if (noDueDate.checked) {
                    d = 0;
                } else {
                    d = app.dateTimeToUnixTime(taskDueDate.value);
                }
                
                var taskData = {"title": taskName.text,
                            "duedate": d,
                            "note": taskNote.text,
                            "folder": folderDropDown.selectedValue};
                
                app.addTask(taskData);
                mainNavPane.pop();
            }
        }
    ]
    
    ScrollView {
        accessibility.name: "Add task page scrollview"
        scrollViewProperties {
            scrollMode: ScrollMode.Vertical
        }
        Container {
            accessibility.name: "Add task page container"
            layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
            topPadding: 30
            leftPadding: 30
            rightPadding: 30
            bottomPadding: 30
            
            Label {
                id: titleRequired
                text: "Required"
                textStyle.color: Color.Red
                textStyle.fontSize: FontSize.Large
                visible: false
            }
            TextField {
                id: taskName
                hintText: "Task Name"
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 40
            }
            TextArea {
                id: taskNote
                hintText: "Detailed notes about task"
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 40
            }
            CheckBox {
                id: noDueDate
                text: "No Due Date"
                bottomMargin: 40
                onCheckedChanged: {
                    taskDueDate.enabled = ! noDueDate.checked;
                }
            }
            DateTimePicker {
                id: taskDueDate
                horizontalAlignment: HorizontalAlignment.Fill
                mode: DateTimePickerMode.Date
                expanded: false
                title: "Due Date"
                bottomMargin: 40
            }
            DropDown {
                id: folderDropDown
                accessibility.name: "Folder drop-down list"
                Option {
                    text: "No folder"
                    value: 0
                    selected: true
                }
                bottomMargin: 40
            }
        }
    }
     
    attachedObjects: [
        ComponentDefinition {
            id: option
            Option {}
        }
    ]
}