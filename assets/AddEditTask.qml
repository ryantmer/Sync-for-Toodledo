import bb.cascades 1.2

Page {
    id: addEditTaskPage
    
    property variant data;
    property bool edit;
    
    //Called after creating the page to populate fields as required from the passed data
    function setup() {
        var folders = app.folderDataModel.getInternalList();
        
        //Populate options in folder dropdown
        for (var x = 0; x < folders.length; x++) {
            if (folders[x].archived == 0) {
                var opt = option.createObject();
                opt.text = folders[x].name;
                opt.value = folders[x].id;
                folderDropDown.add(opt);
            }
        }
        
        //If we're editing a task, rather than adding a new one
        if (edit) {
            var index;
            
            //Populate the fields of the form as required
            titleField.text = data.title;
            tagField.text = data.tag;
            for (index = 0; index < folders.length; index++) {
                if (data.folder == folderDropDown.options[index].value) {
                    break;
                }
            }
            folderDropDown.setSelectedIndex(index);
            if (data.duedate == 0) {
                duedateCheckbox.checked = true;
                duedatePicker.enabled = false;
            } else {
                duedateCheckbox.checked = false;
                duedatePicker.enabled = true;
                duedatePicker.value = app.unixTimeToDateTime(data.duedate);
            }
            duedatemodDropDown.setSelectedIndex(data.duedatemod);
            if (data.startdate != 0) {
                startdatePicker.value = app.unixTimeToDateTime(data.startdate);
                startdateCheckbox.checked = false;
            } else {
                startdateCheckbox.checked = true;
            }
            if (data.duetime != 0) {
                duetimePicker.value = app.unixTimeToDateTimeNoOffset(data.duetime);
                duetimeCheckbox.checked = false;
            } else {
                duetimeCheckbox.checked = true;
            }
            if (data.starttime != 0) {
                starttimePicker.value = app.unixTimeToDateTimeNoOffset(data.starttime);
                starttimeCheckbox.checked = false;
            } else {
                starttimeCheckbox.checked = true;
            }
            for (index = 0; index < remindDropDown.options.length; index++) {
                if (data.remind == remindDropDown.options[index].value) {
                    break;
                }
            }
            remindDropDown.setSelectedIndex(index);
            for (index = 0; index < repeatDropDown.options.length; index++) {
                if (data.repeat == repeatDropDown.options[index].value) {
                    break;
                }
            }
            repeatDropDown.setSelectedIndex(index);
            for (index = 0; index < statusDropDown.options.length; index++) {
                if (data.status == statusDropDown.options[index].value) {
                    break;
                }
            }
            statusDropDown.setSelectedIndex(index);
            if (data.length != 0) {
                lengthPicker.value = new Date(0, 0, 0, 0, data.length);
                lengthCheckbox.checked = false;
            } else {
                lengthCheckbox.checked = true;
            }
            priorityDropDown.setSelectedIndex(data.priority + 1);
            starToggle.checked = data.star;
            completedCheckbox.checked = data.completed;
            noteArea.text = data.note;
            
            //Set title/icon for Add/Save button
            addSaveButton.title = "Save"
            addSaveButton.imageSource = "asset:///images/ic_save.png"
        } else {
            //If adding a new task, set up default values for everything
            duetimeCheckbox.checked = true;
            startdateCheckbox.checked = true;
            starttimeCheckbox.checked = true;
            lengthCheckbox.checked = true;
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
                if (!titleField.text) {
                    titleRequiredLabel.visible = true;
                    return;
                }
                
                var taskData = {}
                if (edit) {
                    //Editing a pre-existing task
                    taskData = data;
                    taskData["completed"] = completedCheckbox.checked ?
                                Math.floor((new Date()).getTime() / 1000) : 0;
                }
                
                taskData["title"] = titleField.text;
                taskData["note"] = noteArea.text;
                taskData["duedate"] = duedateCheckbox.checked ? 
                            0: app.dateTimeToUnixTime(duedatePicker.value);
                taskData["folder"] = folderDropDown.selectedValue;
                
                if (advancedOptionsToggle.checked) {
                    taskData["tag"] = tagField.text;
                    taskData["duedatemod"] = duedatemodDropDown.selectedValue;
                    taskData["duetime"] = duetimeCheckbox.checked ?
                                0 : app.dateTimeToUnixTimeNoOffset(duetimePicker.value);
                    taskData["startdate"] = startdateCheckbox.checked ?
                                0 : app.dateTimeToUnixTime(startdatePicker.value);
                    taskData["starttime"] = starttimeCheckbox.checked ?
                                0 : app.dateTimeToUnixTimeNoOffset(starttimePicker.value);
                    taskData["length"] = lengthCheckbox.checked ?
                                0 : app.getLengthValue(lengthPicker.value);
                    taskData["remind"] = remindDropDown.selectedValue;
                    taskData["repeat"] = repeatDropDown.selectedValue;
                    taskData["status"] = statusDropDown.selectedValue;
                    taskData["priority"] = priorityDropDown.selectedValue;
                    taskData["star"] = starToggle.checked + 0;
                }
                
//                for (var param in taskData) {
//                    console.log("taskData." + param + " = " + taskData[param]);
//                }
                
                if (edit) {
                    app.taskDataModel.edit(data, taskData);
                } else {
                    app.taskDataModel.add(taskData);
                }
                
                mainNavPane.pop();
            }
        }
    ]
    
    ScrollView {
        scrollViewProperties {
            scrollMode: ScrollMode.Vertical
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            topPadding: 20
            leftPadding: 20
            rightPadding: 20
            bottomPadding: 20
            
            //completed
            CheckBox {
                id: completedCheckbox
                text: "Completed"
                bottomMargin: 30
                visible: edit  //only show if we're editing a task, not adding a new one
            }
            
            //title
            Container {
                Label {
                    id: titleRequiredLabel
                    text: "Required"
                    textStyle.color: Color.Red
                    visible: false
                }
                TextField {
                    id: titleField
                    hintText: "Task Name"
                    horizontalAlignment: HorizontalAlignment.Fill
                }
            }
            
            //note
            TextArea {
                id: noteArea
                hintText: "Detailed notes about task"
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 30
            }
            
            //duedate
            Container {
                bottomMargin: 30
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                DateTimePicker {
                    id: duedatePicker
                    horizontalAlignment: HorizontalAlignment.Fill
                    mode: DateTimePickerMode.Date
                    expanded: false
                    title: "Due Date"
                }
                CheckBox {
                    id: duedateCheckbox
                    text: "None"
                    verticalAlignment: VerticalAlignment.Center
                    onCheckedChanged: {
                        duedatePicker.enabled = !checked;
                    }
                }
            }
            
            //folder
            DropDown {
                id: folderDropDown
                title: "Folder"
                bottomMargin: 30
                Option {
                    text: "No folder"
                    value: 0
                    selected: true
                }
            }
            
            //advanced options stuff
            Container {
                bottomMargin: 30
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    text: "Additional Options"
                    textStyle.fontSize: FontSize.XLarge
                    verticalAlignment: VerticalAlignment.Center
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                ToggleButton {
                    id: advancedOptionsToggle
                    verticalAlignment: VerticalAlignment.Center
                    onCheckedChanged: {
                        advancedContainer.visible = checked;
                    }
                }
            }
            Container {
                id: advancedContainer
                visible: false
                
                //tag
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Label {
                        text: "Tags"
                        textStyle.fontSize: FontSize.XLarge
                        verticalAlignment: VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                    }
                    TextField {
                        id: tagField
                        hintText: "Comma-separated tags for this task"
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 3
                        }
                    }
                }
                
                //duedatemod
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Label {
                        text: "Due"
                        textStyle.fontSize: FontSize.XLarge
                        verticalAlignment: VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                    }
                    DropDown {
                        id: duedatemodDropDown
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 3
                        }
                        options: [
                            Option {
                                text: "By Due Date"
                                value: 0
                                selected: true
                            },
                            Option {
                                text: "On Due Date"
                                value: 1
                            },
                            Option {
                                text: "After Due Date"
                                value: 2
                            },
                            Option {
                                text: "Optional"
                                value: 3
                            }
                        ]
                    }
                    
                }
                
                //duetime
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    DateTimePicker {
                        id: duetimePicker
                        horizontalAlignment: HorizontalAlignment.Fill
                        mode: DateTimePickerMode.Time
                        expanded: false
                        title: "Due Time"
                    }
                    CheckBox {
                        id: duetimeCheckbox
                        text: "None"
                        verticalAlignment: VerticalAlignment.Center
                        onCheckedChanged: {
                            duetimePicker.enabled = !checked;
                        }
                    }
                }
                
                //startdate
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    DateTimePicker {
                        id: startdatePicker
                        horizontalAlignment: HorizontalAlignment.Fill
                        mode: DateTimePickerMode.Date
                        expanded: false
                        title: "Start Date"
                    }
                    CheckBox {
                        id: startdateCheckbox
                        text: "None"
                        verticalAlignment: VerticalAlignment.Center
                        onCheckedChanged: {
                            startdatePicker.enabled = !checked;
                        }
                    }
                }
                
                //starttime
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    DateTimePicker {
                        id: starttimePicker
                        horizontalAlignment: HorizontalAlignment.Fill
                        mode: DateTimePickerMode.Time
                        expanded: false
                        title: "Start Time"
                    }
                    CheckBox {
                        id: starttimeCheckbox
                        text: "None"
                        verticalAlignment: VerticalAlignment.Center
                        onCheckedChanged: {
                            starttimePicker.enabled = !checked;
                        }
                    }
                }
                
                //length
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    DateTimePicker {
                        id: lengthPicker
                        horizontalAlignment: HorizontalAlignment.Fill
                        mode: DateTimePickerMode.Timer
                        expanded: false
                        title: "Task Length"
                    }
                    CheckBox {
                        id: lengthCheckbox
                        text: "None"
                        verticalAlignment: VerticalAlignment.Center
                        onCheckedChanged: {
                            lengthPicker.enabled = !checked;
                        }
                    }
                }
                
                //remind
                DropDown {
                    id: remindDropDown
                    title: "Reminder"
                    bottomMargin: 30
                    options: [
                        Option {
                            text: "No Reminder"
                            value: 0
                        },
                        Option {
                            text: "1 Minute"
                            value: 1
                        },
                        Option {
                            text: "15 Minutes"
                            value: 15
                        },
                        Option {
                            text: "30 Minutes"
                            value: 30
                        },
                        Option {
                            text: "60 Minutes"
                            value: 60
                        },
                        Option {
                            text: "90 Minutes"
                            value: 90
                        },
                        Option {
                            text: "2 Hours"
                            value: 120
                        },
                        Option {
                            text: "3 Hours"
                            value: 180
                        },
                        Option {
                            text: "4 Hours"
                            value: 240
                        },
                        Option {
                            text: "1 Day"
                            value: 1440
                        },
                        Option {
                            text: "2 Days"
                            value: 2880
                        },
                        Option {
                            text: "3 Days"
                            value: 4320
                        },
                        Option {
                            text: "4 Days"
                            value: 5760
                        },
                        Option {
                            text: "5 Days"
                            value: 7200
                        },
                        Option {
                            text: "6 Days"
                            value: 8640
                        },
                        Option {
                            text: "1 Week"
                            value: 10080
                        },
                        Option {
                            text: "2 Weeks"
                            value: 20160
                        },
                        Option {
                            text: "1 Month"
                            value: 43200
                        }
                    ]
                }
                
                //repeat
                DropDown {
                    id: repeatDropDown
                    title: "Repeat"
                    bottomMargin: 30
                    options: [
                        Option {
                            text: "Don't Repeat"
                            value: ""
                        },
                        Option {
                            text: "Daily"
                            value: "FREQ=DAILY"
                        },
                        Option {
                            text: "Weekly"
                            value: "FREQ=WEEKLY"
                        },
                        Option {
                            text: "Biweekly"
                            value: "FREQ=WEEKLY;INTERVAL=2"
                        },
                        Option {
                            text: "Monthly"
                            value: "FREQ=MONTHLY"
                        },
                        Option {
                            text: "Bimonthly"
                            value: "FREQ=MONTHLY;INTERVAL=2"
                        },
                        Option {
                            text: "Quarterly"
                            value: "FREQ=MONTHLY;INTERVAL=3"
                        },
                        Option {
                            text: "Semiannually"
                            value: "FREQ=MONTHLY;INTERVAL=6"
                        },
                        Option {
                            text: "Yearly"
                            value: "FREQ=YEARLY"
                        }
                    ]
                }
                
                //status
                DropDown {
                    id: statusDropDown
                    title: "Status"
                    bottomMargin: 30
                    options: [
                        Option {
                            text: "None"
                            value: 0
                        },
                        Option {
                            text: "Next Action"
                            value: 1
                        },
                        Option {
                            text: "Active"
                            value: 2
                        },
                        Option {
                            text: "Planning"
                            value: 3
                        },
                        Option {
                            text: "Delegated"
                            value: 4
                        },
                        Option {
                            text: "Waiting"
                            value: 5
                        },
                        Option {
                            text: "Hold"
                            value: 6
                        },
                        Option {
                            text: "Postponed"
                            value: 7
                        },
                        Option {
                            text: "Someday"
                            value: 8
                        },
                        Option {
                            text: "Canceled"
                            value: 9
                        },
                        Option {
                            text: "Reference"
                            value: 10
                        }
                    ]
                }
                
                //priority
                DropDown {
                    id: priorityDropDown
                    title: "Priority"
                    bottomMargin: 30
                    options: [
                        Option {
                            text: "-1 Negative"
                            value: -1
                        },
                        Option {
                            text: "0 Low"
                            value: 0
                        },
                        Option {
                            text: "1 Medium"
                            value: 1
                        },
                        Option {
                            text: "2 High"
                            value: 2
                        },
                        Option {
                            text: "3 Top"
                            value: 3
                        }
                    ]
                }
                
                //star
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Label {
                        text: "Star"
                        textStyle.fontSize: FontSize.XLarge
                        verticalAlignment: VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                    }
                    ToggleButton {
                        id: starToggle
                        verticalAlignment: VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                    }
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
