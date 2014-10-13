import bb.cascades 1.2

Page {
    id: editTaskPage
    accessibility.name: "Edit task page"
     
    property variant data;
        
    function populateFolderDropDown() {
        var folders = app.getFolderList();
        
        var index;
        for (var x = 0; x < folders.length; x++) {
            var opt = option.createObject();
            opt.text = folders[x].name;
            opt.value = folders[x].id;
            folderDropDown.add(opt);
            if (opt.value == data.folder) {
                index = x + 1; //Need to account for the first option, No Folder
            }
        }
        
        folderDropDown.setSelectedIndex(index);
        
        taskIdField.text = data.id;
        taskCompleted.checked = data.completed
        taskName.text = data.title;
        if (data.duedate == 0) {
            noDueDate.checked = true;
            taskDueDate.enabled = false;
        } else {
            noDueDate.checked = false;
            taskDueDate.value = app.unixTimeToDateTime(data.duedate);
        }
        taskNote.text = data.note;
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
            title: "Save"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_save.png"
            onTriggered: {
                var c;
                var d;
                
                if (!taskName.text) {
                    titleRequired.visible = true;
                    return;
                }
                
                if (taskCompleted.checked) {
                    c = Math.floor((new Date()).getTime() / 1000);
                } else {
                    c = 0;
                }
                if (noDueDate.checked) {
                    d = 0
                } else {
                    d = app.dateTimeToUnixTime(taskDueDate.value);
                }
                                    
                var taskData = {"id": parseInt(taskIdField.text),
                            "completed": c,
                            "title": taskName.text,
                            "duedate": d,
                            "folder": folderDropDown.selectedValue,
                            "note": taskNote.text};
                app.editTask(data, taskData);
                
                mainNavPane.pop();
            }
        }
    ]
    
    ScrollView {
        accessibility.name: "Edit task page scrollview"
        scrollViewProperties {
            scrollMode: ScrollMode.Vertical
        }
        Container {
            accessibility.name: "Edit task page container"
            layout: StackLayout { orientation: LayoutOrientation.TopToBottom }
            topPadding: 30
            leftPadding: 30
            rightPadding: 30
            bottomPadding: 30
            
            CheckBox {
                id: taskCompleted
                text: "Completed"
                bottomMargin: 40
            }
            Label {
                id: titleRequired
                text: "Required"
                textStyle.color: Color.Red
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
                    taskDueDate.enabled = !noDueDate.checked;
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
            TextField {
                id: taskIdField
                visible: false
                bottomMargin: 40
                accessibility.name: "Hidden textfield containing task ID number"
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