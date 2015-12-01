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

                app.tasks.add(data);

                listNavPane.pop();
            }
        }
    ]
    onCreationCompleted: {
        // Populate FGCL dropdowns dynamically
        var folders = app.folders.getInternalList();
        for (var x = 0; x < folders.length; ++ x) {
            if (folders[x].archived == 0) {
                var opt = option.createObject();
                opt.text = folders[x].name;
                opt.value = folders[x].id;
                folderDropdown.add(opt);
            }
        }

        var goals = app.goals.getInternalList();
        for (var x = 0; x < goals.length; ++ x) {
            if (goals[x].archived == 0) {
                var opt = option.createObject();
                opt.text = goals[x].name;
                opt.value = goals[x].id;
                goalsDropdown.add(opt);
            }
        }

        var contexts = app.contexts.getInternalList();
        for (var x = 0; x < contexts.length; ++ x) {
            var opt = option.createObject();
            opt.text = contexts[x].name;
            opt.value = contexts[x].id;
            contextDropdown.add(opt);
        }

        var locations = app.locations.getInternalList();
        for (var x = 0; x < locations.length; ++ x) {
            var opt = option.createObject();
            opt.text = locations[x].name;
            opt.value = locations[x].id;
            locationDropdown.add(opt);
        }
    }

    function setup() {
        titleField.text = data.title;
        noteArea.text = data.note;

        if (data.duedate == 0) {
            duedateCheckbox.checked = false;
            duedateCheckbox.checkedChanged(false);
        } else {
            duedateCheckbox.checked = true;
            duedateCheckbox.checkedChanged(true);
            duedatePicker.value = app.unixTimeToDateTime(data.duedate);
        }
    }

    ScrollView {
        scrollViewProperties.scrollMode: ScrollMode.Vertical

        Container {
                layout: StackLayout {
                    orientation:
                    LayoutOrientation.TopToBottom
                }
                topPadding:
                20
                leftPadding:
                20
                rightPadding:
                20
                bottomPadding:
                20

            // title
            Container {
                    layout: StackLayout {
                        orientation:
                        LayoutOrientation.TopToBottom
                    }
                    bottomMargin:
                    30

                Label {
                        id:
                        titleRequiredLabel
                        text:
                        "Required"
                    textStyle.color: Color.Red
                        visible:
                        false
                    }
                TextField {
                        id:
                        titleField
                        hintText:
                        "Task Name"
                        horizontalAlignment:
                        HorizontalAlignment.Fill
                        maximumLength:
                        255
                    }
                }
            // note
            TextArea {
                    id:
                    noteArea
                    hintText:
                    "Description and notes about this task"
                    horizontalAlignment:
                    HorizontalAlignment.Fill
                    maximumLength:
                    32000
                    bottomMargin:
                    30
                }
            // duedate
            Container {
                    layout: StackLayout {
                        orientation:
                        LayoutOrientation.LeftToRight
                    }
                    bottomMargin:
                    30

                CheckBox {
                        id:
                        duedateCheckbox
                        verticalAlignment:
                        VerticalAlignment.Center
                        onCheckedChanged:
                        {
                            duedatePicker.enabled = checked;
                        }
                    }
                DateTimePicker {
                        id:
                        duedatePicker
                        title:
                        "Due Date"
                    }
                }
            // folder
            DropDown {
                    id:
                    folderDropdown
                    title:
                    "Folder"
                    bottomMargin:
                    30
                Option {
                        text:
                        "No folder"
                        value:
                        0
                        selected:
                        true
                    }
                }
            // advanced options toggle
            Container {
                    layout: StackLayout {
                        orientation:
                        LayoutOrientation.LeftToRight
                    }
                    bottomMargin:
                    30

                Label {
                        text:
                        "Additional Options"
                        verticalAlignment:
                        VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota:
                            1
                        }
                    }
                ToggleButton {
                        id:
                        advancedToggle
                        verticalAlignment:
                        VerticalAlignment.Center
                        onCheckedChanged:
                        {
                            advancedContainer.visible = checked
                        }
                    }
                }
            // advanced options container
            Container {
                    id:
                    advancedContainer
                    visible:
                    false

                // star
                Container {
                        bottomMargin:
                        30
                        layout: StackLayout {
                            orientation:
                            LayoutOrientation.LeftToRight
                        }
                    Label {
                            text:
                            "Star"
                            verticalAlignment:
                            VerticalAlignment.Center
                            layoutProperties: StackLayoutProperties {
                                spaceQuota:
                                1
                            }
                        }
                    ToggleButton {
                            id:
                            starToggle
                            verticalAlignment:
                            VerticalAlignment.Center
                            layoutProperties: StackLayoutProperties {
                                spaceQuota:
                                1
                            }
                        }
                    }
                // context
                DropDown {
                        id:
                        contextDropdown
                        title:
                        "Context"
                        bottomMargin:
                        30
                    Option {
                            text:
                            "None"
                            value:
                            0
                        }
                    }
                // goal
                DropDown {
                        id:
                        goalDropdown
                        title:
                        "Goal"
                    Option {
                            text:
                            "None"
                            value:
                            0
                        }
                    }
                // location
                DropDown {
                        id:
                        locationDropdown
                        title:
                        "Location"
                    Option {
                            text:
                            "None"
                            value:
                            0
                        }
                    }
                // tag
                TextArea {
                        id:
                        tagField
                        hintText:
                        "Comma-separated Tags for this task"
                    }
                // duedatemod
                DropDown {
                        id:
                        duedatemodDropdown
                        title:
                        "Due"
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
                        bottomMargin:
                        30
                        layout: StackLayout {
                            orientation:
                            LayoutOrientation.LeftToRight
                        }
                    CheckBox {
                            id:
                            duetimeCheckbox
                            verticalAlignment:
                            VerticalAlignment.Center
                            onCheckedChanged:
                            {
                                duetimePicker.enabled = checked;
                            }
                        }
                    DateTimePicker {
                            id:
                            duetimePicker
                            title:
                            "Due Time"
                            mode:
                            DateTimePickerMode.Time
                        }
                    }
                // startdate
                Container {
                        bottomMargin:
                        30
                        layout: StackLayout {
                            orientation:
                            LayoutOrientation.LeftToRight
                        }
                    CheckBox {
                            id:
                            startdateCheckbox
                            verticalAlignment:
                            VerticalAlignment.Center
                            onCheckedChanged:
                            {
                                startdatePicker.enabled = checked;
                            }
                        }
                    DateTimePicker {
                            id:
                            startdatePicker
                            title:
                            "Start Date"
                        }
                    }
                // starttime
                Container {
                        bottomMargin:
                        30
                        layout: StackLayout {
                            orientation:
                            LayoutOrientation.LeftToRight
                        }
                    CheckBox {
                            id:
                            starttimeCheckbox
                            verticalAlignment:
                            VerticalAlignment.Center
                            onCheckedChanged:
                            {
                                starttimePicker.enabled = checked;
                            }
                        }
                    DateTimePicker {
                            id:
                            starttimePicker
                            title:
                            "Start Time"
                            mode:
                            DateTimePickerMode.Time
                        }
                    }
                // length
                Container {
                        bottomMargin:
                        30
                        layout: StackLayout {
                            orientation:
                            LayoutOrientation.LeftToRight
                        }
                    CheckBox {
                            id:
                            lengthCheckbox
                            verticalAlignment:
                            VerticalAlignment.Center
                            onCheckedChanged:
                            {
                                lengthPicker.enabled = checked;
                            }
                        }
                    DateTimePicker {
                            id:
                            lengthPicker
                            title:
                            "Task Length"
                            mode:
                            DateTimePickerMode.Timer
                        }
                    }
                // remind
                DropDown {
                        id:
                        remindDropdown
                        title:
                        "Reminder"
                        bottomMargin:
                        30
                    // Other length reminders are a premium-only feature
                    Option {
                            text:
                            "60 minutes"
                            value:
                            60
                        }
                    }
                // repeat
                DropDown {
                        id:
                        repeatDropdown
                        title:
                        "Repeat"
                        bottomMargin:
                        30
                        options: [
                        Option {
                            text: "Don't repeat"
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
                // status
                DropDown {
                        id:
                        statusDropdown
                        title:
                        "Status"
                        bottomMargin:
                        30
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
                        id:
                        priorityDropdown
                        title:
                        "Priority"
                        bottomMargin:
                        30
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
                }
            }
        }
    }
