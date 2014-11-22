import bb.cascades 1.2

Page {
    id: addEditTaskPage
    accessibility.name: "Add/edit task page"
    
    property variant data;
    property bool edit;
    
    function setup() {
        var folders = app.getFolderList();
        
        //Populate options in folder dropdown
        for (var x = 0; x < folders.length; x++) {
            var opt = option.createObject();
            opt.text = folders[x].name;
            opt.value = folders[x].id;
            folderDropDown.add(opt);
        }
        
        //If we're editing a task, rather than adding a new one
        if (edit) {
            //Pre-select the task's folder in the list
            var index;
            for (var x = 0; x < folders.length; x++) {
                if (data.folder == folderDropDown.options[x].value) {
                    index = x;
                    break;
                }
            }
            folderDropDown.setSelectedIndex(index);
            
            //Populate the fields of the form as required
            taskCompleted.checked = data.completed;
            taskName.text = data.title;
            if (data.duedate == 0) {
                noDueDate.checked = true;
                taskDueDate.enabled = false;
            } else {
                noDueDate.checked = false;
                taskDueDate.enabled = true;
                taskDueDate.value = app.unixTimeToDateTime(data.duedate);
            }
            taskNote.text = data.note;
            
            //Set title/icon for Add/Save button
            addSaveButton.title = "Save"
            addSaveButton.imageSource = "asset:///images/ic_save.png"
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
            id: addSaveButton
            title: "Add" //Changed to "Save" in setup if editing a task
            imageSource: "asset:///images/ic_add.png" //Changed in setup if editing a task
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                var c = 0;
                var d = 0;
                
                if (!taskName.text) {
                    titleRequired.visible = true;
                    return;
                }
                if (taskCompleted.checked) {
                    c = Math.floor((new Date()).getTime() / 1000);
                }
                if (!noDueDate.checked) {
                    d = app.dateTimeToUnixTime(taskDueDate.value);
                }
                
                if (edit) {
                    //Editing a pre-existing task
                    var taskData = {"id": data.id,
                                "completed": c,
                                "title": taskName.text,
                                "duedate": d,
                                "folder": folderDropDown.selectedValue,
                                "note": taskNote.text};
                    app.editTask(data, taskData);
                } else {
                    //Adding a new task
                    var taskData = {"title": taskName.text,
                                "duedate": d,
                                "note": taskNote.text,
                                "folder": folderDropDown.selectedValue};
                    app.addTask(taskData);
                }
                
                mainNavPane.pop();
            }
        }
    ]
    
    ScrollView {
        accessibility.name: "Add/edit task page scrollview"
        scrollViewProperties {
            scrollMode: ScrollMode.Vertical
        }
        Container {
            accessibility.name: "Add/edit task page container"
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            topPadding: 20
            leftPadding: 20
            rightPadding: 20
            bottomPadding: 20
            
            CheckBox {
                id: taskCompleted
                text: "Completed"
                bottomMargin: 30
                visible: edit  //only show if we're editing a task, not adding a new one
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
                bottomMargin: 30
            }
            TextArea {
                id: taskNote
                hintText: "Detailed notes about task"
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 30
            }
            DateTimePicker {
                id: taskDueDate
                horizontalAlignment: HorizontalAlignment.Fill
                mode: DateTimePickerMode.Date
                expanded: false
                title: "Due Date"
                bottomMargin: 30                
            }
            CheckBox {
                id: noDueDate
                text: "No Due Date"
                bottomMargin: 30
                onCheckedChanged: {
                    taskDueDate.enabled = !noDueDate.checked;
                }
            }
            DropDown {
                id: folderDropDown
                accessibility.name: "Folder drop-down list"
                bottomMargin: 30
                Option {
                    text: "No folder"
                    value: 0
                    selected: true
                }
            }
        }
    }
    
    attachedObjects: [
        ComponentDefinition {
            //Used to populate folder drop-down list
            id: option
            Option {}
        }
    ]
}
