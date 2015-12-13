import bb.cascades 1.4

Page {
    id: editTaskPage
    property variant data

    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: "Cancel"
            onTriggered: {
                listNavPane.pop();
            }
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: option
            Option {
            }
        }
    ]
    actions: [
        ActionItem {
            id: saveButton
            title: "Save"
            imageSource: "asset:///images/ic_save.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (! titleField.text) {
                    titleRequiredLabel.visible = true;
                    return;
                }

                var newData = data;

                newData.title = titleField.text;
                newData.note = noteArea.text;

                app.tasks.edit(data, newData);

                listNavPane.pop();
            }
        }
    ]

    function setup() {
        var index = 0;

        titleField.text = data.title;
        noteArea.text = data.note;
        tagField.text = data.tag;
        duedatemodDropdown.setSelectedIndex(data.duedatemod);
        priorityDropdown.setSelectedIndex(data.priority + 1);
        starToggle.checked = data.star;

        if (data.duedate != 0) {
            duedatePicker.value = app.unixTimeToDateTime(data.duedate);
            duedateCheckbox.checked = true;
            duedateCheckbox.checkedChanged(true);
        }

        if (data.duetime != 0) {
            duetimePicker.value = app.unixTimeToDateTimeNoOffset(data.duetime);
            duetimeCheckbox.checked = true;
            duetimeCheckbox.checkedChanged(true);
        }

        if (data.startdate != 0) {
            startdatePicker.value = app.unixTimeToDateTime(data.startdate);
            startdateCheckbox.checked = true;
            startdateCheckbox.checkedChanged(true);
        }

        if (data.starttime != 0) {
            starttimePicker.value = app.unixTimeToDateTimeNoOffset(data.starttime);
            starttimeCheckbox.checked = true;
            starttimeCheckbox.checkedChanged(true);
        }

        if (data.length != 0) {
            lengthPicker.value = new Date(0, 0, 0, 0, data.length);
            lengthCheckbox.checked = true;
            lengthCheckbox.checkedChanged(true);
        }

        if (data.remind != 0) {
            for (index = 0; index < remindDropdown.options.length; ++ index) {
                if (data.remind == remindDropdown.options[index].value) {
                    remindDropdown.setSelectedIndex(index);
                    break;
                }
            }
        }

        if (data.repeat != 0) {
            for (index = 0; index < repeatDropdown.options.length; ++ index) {
                if (data.repeat == repeatDropdown.options[index].value) {
                    repeatDropdown.setSelectedIndex(index);
                    break;
                }
            }
        }

        if (data.status != 0) {
            for (index = 0; index < statusDropdown.options.length; ++ index) {
                if (data.status == statusDropdown.options[index].value) {
                    statusDropdown.setSelectedIndex(index);
                    break;
                }
            }
        }
        
        // Fill out FCGL dropdowns
        for (var i = 0; i < app.data.allData.length; ++i) {
            var item = app.data.allData[i];
            if (item.type == "folders") {
                if (item.archived != 0) {
                    continue;
                }
                var opt = option.createObject();
                opt.text = item.name;
                opt.value = item.id;
                folderDropdown.add(opt);
            } else if (item.type == "goals") {
                if (item.archived != 0) {
                    continue;
                }
                var opt = option.createObject();
                opt.text = item.name;
                opt.value = item.id;
                goalDropdown.add(opt);
            } else if (item.type == "contexts") {
                var opt = option.createObject();
                opt.text = item.name;
                opt.value = item.id;
                contextDropdown.add(opt);
            } else if (item.type == "locations") {
                var opt = option.createObject();
                opt.text = item.name;
                opt.value = item.id;
                locationDropdown.add(opt);
            }
        }
        
        // Set FCGL options to be whatever the task being edited has
        if (data.folder != 0) {
            for (index = 0; index < folderDropdown.options.length; ++index) {
                if (data.folder == folderDropdown.options[index].value) {
                    folderDropdown.setSelectedIndex(index);
                }
            }
        }
        if (data.goal != 0) {
            for (index = 0; index < goalDropdown.options.length; ++index) {
                if (data.goal == goalDropdown.options[index].value) {
                    goalDropdown.setSelectedIndex(index);
                }
            }
        }
        if (data.context != 0) {
            for (index = 0; index < contextDropdown.options.length; ++index) {
                if (data.context == contextDropdown.options[index].value) {
                    contextDropdown.setSelectedIndex(index);
                }
            }
        }
        if (data.location != 0) {
            for (index = 0; index < locationDropdown.options.length; ++index) {
                if (data.location == locationDropdown.options[index].value) {
                    locationDropdown.setSelectedIndex(index);
                }
            }
        }
    }

    ScrollView {
        scrollViewProperties.scrollMode: ScrollMode.Vertical

        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            topPadding: 20
            leftPadding: 20
            rightPadding: 20
            bottomPadding: 20

            // title
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                bottomMargin: 30

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
                    maximumLength: 255
                }
            }
            // note
            TextArea {
                id: noteArea
                hintText: "Description and notes about this task"
                horizontalAlignment: HorizontalAlignment.Fill
                maximumLength: 32000
                bottomMargin: 30
            }
            // duedate
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                bottomMargin: 30

                CheckBox {
                    id: duedateCheckbox
                    verticalAlignment: VerticalAlignment.Center
                    onCheckedChanged: {
                        duedatePicker.enabled = checked;
                    }
                }
                DateTimePicker {
                    id: duedatePicker
                    title: "Due Date"
                    enabled: false
                }
            }
            // folder
            DropDown {
                id: folderDropdown
                title: "Folder"
                bottomMargin: 30
                Option {
                    text: "No folder"
                    value: 0
                    selected: true
                }
            }
            // advanced options toggle
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                bottomMargin: 30

                Label {
                    text: "Additional Options"
                    verticalAlignment: VerticalAlignment.Center
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                ToggleButton {
                    id: advancedToggle
                    verticalAlignment: VerticalAlignment.Center
                    onCheckedChanged: {
                        advancedContainer.visible = checked
                    }
                }
            }
            // advanced options container
            Container {
                id: advancedContainer
                visible: false

                // star
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Label {
                        text: "Star"
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
                // context
                DropDown {
                    id: contextDropdown
                    title: "Context"
                    bottomMargin: 30
                    Option {
                        text: "None"
                        value: 0
                    }
                }
                // goal
                DropDown {
                    id: goalDropdown
                    title: "Goal"
                    Option {
                        text: "None"
                        value: 0
                    }
                }
                // location
                DropDown {
                    id: locationDropdown
                    title: "Location"
                    Option {
                        text: "None"
                        value: 0
                    }
                }
                // tag
                TextArea {
                    id: tagField
                    hintText: "Comma-separated Tags for this task"
                }
                // duedatemod
                DropDown {
                    id: duedatemodDropdown
                    title: "Due"
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
                // duetime
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    CheckBox {
                        id: duetimeCheckbox
                        verticalAlignment: VerticalAlignment.Center
                        onCheckedChanged: {
                            duetimePicker.enabled = checked;
                        }
                    }
                    DateTimePicker {
                        id: duetimePicker
                        title: "Due Time"
                        mode: DateTimePickerMode.Time
                        enabled: false
                    }
                }
                // startdate
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    CheckBox {
                        id: startdateCheckbox
                        verticalAlignment: VerticalAlignment.Center
                        onCheckedChanged: {
                            startdatePicker.enabled = checked;
                        }
                    }
                    DateTimePicker {
                        id: startdatePicker
                        title: "Start Date"
                        enabled: false
                    }
                }
                // starttime
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    CheckBox {
                        id: starttimeCheckbox
                        verticalAlignment: VerticalAlignment.Center
                        onCheckedChanged: {
                            starttimePicker.enabled = checked;
                        }
                    }
                    DateTimePicker {
                        id: starttimePicker
                        title: "Start Time"
                        mode: DateTimePickerMode.Time
                        enabled: false
                    }
                }
                // length
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    CheckBox {
                        id: lengthCheckbox
                        verticalAlignment: VerticalAlignment.Center
                        onCheckedChanged: {
                            lengthPicker.enabled = checked;
                        }
                    }
                    DateTimePicker {
                        id: lengthPicker
                        title: "Task Length"
                        mode: DateTimePickerMode.Timer
                        enabled: false
                    }
                }
                // remind
                DropDown {
                    id: remindDropdown
                    title: "Reminder"
                    bottomMargin: 30
                    options: [
                        Option {
                            text: "None"
                            value: 0
                            selected: true
                        },
                        // Other length reminders are a premium-only feature
                        Option {
                            text: "60 minutes"
                            value: 60
                        }
                    ]
                }
                // repeat
                DropDown {
                    id: repeatDropdown
                    title: "Repeat"
                    bottomMargin: 30
                    options: [
                        Option {
                            text: "Don't repeat"
                            value: ""
                            selected: true
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
                // status
                DropDown {
                    id: statusDropdown
                    title: "Status"
                    bottomMargin: 30
                    options: [
                        Option {
                            text: "None"
                            value: 0
                            selected: true
                        },
                        Option {
                            text: "Next Action"
                            value: 1
                        },
                        Option {
                            text: "Planning"
                            value: 2
                        },
                        Option {
                            text: "Delegated"
                            value: 3
                        },
                        Option {
                            text: "Waiting"
                            value: 4
                        },
                        Option {
                            text: "Hold"
                            value: 5
                        },
                        Option {
                            text: "Postponed"
                            value: 6
                        },
                        Option {
                            text: "Someday"
                            value: 7
                        },
                        Option {
                            text: "Canceled"
                            value: 8
                        },
                        Option {
                            text: "Reference"
                            value: 9
                        }
                    ]
                }
                // priority
                DropDown {
                    id: priorityDropdown
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
                            selected: true
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
            }
        }
    }
}
