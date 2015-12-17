import bb.cascades 1.4

Page {
    id: addTaskPage
    
    attachedObjects: [
        ComponentDefinition {
            id: option
            Option {
            }
        }
    ]

    actions: [
        ActionItem {
            id: addButton
            title: "Add"
            imageSource: "asset:///images/ic_add.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (! titleField.text) {
                    titleRequiredLabel.visible = true;
                    return;
                }

                var data = {
                };

                // Only insert non-blank/non-default values
                // All dem text values
                data.title = titleField.text;
                if (noteArea.text.length > 0) {
                    data.note = noteArea.text;
                }
                if (tagField.text.length > 0) {
                    data.tag = tagField.text;
                }
                // All dem pickers
                if (duedateSegmented.selectedValue == "yes") {
                    data.duedate = app.dateTimeToUnixTime(duedatePicker.value);
                }
                if (duetimeSegmented.selectedValue == "yes") {
                    data.duetime = app.dateTimeToUnixTime(duetimePicker.value);
                }
                if (startdateSegmented.selectedValue == "yes") {
                    data.startdate = app.dateTimeToUnixTime(startdatePicker.value);
                }
                if (starttimeSegmented.selectedValue == "yes") {
                    data.starttime = app.dateTimeToUnixTime(starttimePicker.value);
                }
                if (lengthSegmented.selectedValue == "yes") {
                    data.length = app.getLengthValue(lengthPicker.value);
                }
                // All dem dropdowns
                if (folderDropdown.selectedValue != 0) {
                    data.folder = folderDropdown.selectedValue;
                }
                if (contextDropdown.selectedValue != 0) {
                    data.context = contextDropdown.selectedValue;
                }
                if (goalDropdown.selectedValue != 0) {
                    data.goal = goalDropdown.selectedValue;
                }
                if (locationDropdown.selectedValue != 0) {
                    data.location = locationDropdown.selectedValue;
                }
                if (duedatemodDropdown.selectedValue != 0) {
                    data.duedatemod = duedatemodDropdown.selectedValue;
                }
                if (remindDropdown.selectedValue != 0) {
                    data.remind = remindDropdown.selectedValue;
                }
                if (repeatDropdown.selectedValue != "") {
                    data.repeat = repeatDropdown.selectedValue;
                }
                if (statusDropdown.selectedValue != 0) {
                    data.status = statusDropdown.selectedValue;
                }
                if (priorityDropdown.selectedValue != 0) {
                    data.priority = priorityDropdown.selectedValue;
                }
                // And the lonely star.
                if (starToggle.checked) {
                    data.star = 1;
                }

                app.data.addItem("tasks", data);

                navigationPane.pop();
            }
        }
    ]

    function setup() {
        // Fill out FCGL dropdowns
        for (var i = 0; i < app.data.allData.length; ++ i) {
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
                    orientation: LayoutOrientation.TopToBottom
                }
                bottomMargin: 30

                SegmentedControl {
                    id: duedateSegmented
                    Option {
                        text: "No Due Date"
                        value: "no"
                    }
                    Option {
                        text: "Set Due Date"
                        value: "yes"
                    }
                    onSelectedIndexChanged: {
                        if (duedateSegmented.selectedValue == "yes") {
                            duedatePicker.setVisible(true);
                        } else {
                            duedatePicker.setVisible(false);
                        }
                    }
                }
                DateTimePicker {
                    id: duedatePicker
                    title: "Due Date"
                    visible: false
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
                        orientation: LayoutOrientation.TopToBottom
                    }
                    SegmentedControl {
                        Option {
                            text: "No Due Time"
                            value: "no"
                        }
                        id: duetimeSegmented
                        Option {
                            text: "Set Due Time"
                            value: "yes"
                        }
                        onSelectedIndexChanged: {
                            if (duetimeSegmented.selectedValue == "yes") {
                                duetimePicker.setVisible(true);
                            } else {
                                duetimePicker.setVisible(false);
                            }
                        }
                    }
                    DateTimePicker {
                        id: duetimePicker
                        title: "Due Time"
                        mode: DateTimePickerMode.Time
                        visible: false
                    }
                }
                // startdate
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    SegmentedControl {
                        Option {
                            text: "No Start Date"
                            value: "no"
                        }
                        id: startdateSegmented
                        Option {
                            text: "Set Start Date"
                            value: "yes"
                        }
                        onSelectedIndexChanged: {
                            if (startdateSegmented.selectedValue == "yes") {
                                startdatePicker.setVisible(true);
                            } else {
                                startdatePicker.setVisible(false);
                            }
                        }
                    }
                    DateTimePicker {
                        id: startdatePicker
                        title: "Start Date"
                        visible: false
                    }
                }
                // starttime
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    SegmentedControl {
                        id: starttimeSegmented
                        Option {
                            text: "No Start Time"
                            value: "no"
                        }
                        Option {
                            text: "Set Start Time"
                            value: "yes"
                        }
                        onSelectedIndexChanged: {
                            if (starttimeSegmented.selectedValue == "yes") {
                                starttimePicker.setVisible(true);
                            } else {
                                starttimePicker.setVisible(false);
                            }
                        }
                    }
                    DateTimePicker {
                        id: starttimePicker
                        title: "Start Time"
                        mode: DateTimePickerMode.Time
                        visible: false
                    }
                }
                // length
                Container {
                    bottomMargin: 30
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    SegmentedControl {
                        id: lengthSegmented
                        Option {
                            text: "No Length"
                            value: "no"
                        }
                        Option {
                            text: "Set Length"
                            value: "yes"
                        }
                        onSelectedIndexChanged: {
                            if (lengthSegmented.selectedValue == "yes") {
                                lengthPicker.setVisible(true);
                            } else {
                                lengthPicker.setVisible(false);
                            }
                        }
                    }
                    DateTimePicker {
                        id: lengthPicker
                        title: "Task Length"
                        mode: DateTimePickerMode.Timer
                        visible: false
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
