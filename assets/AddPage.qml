import bb.cascades 1.4

Page {
    id: addPage
    property string backingDataType
    property variant backingData

    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: "Cancel"
            onTriggered: {
                listNavPane.pop();
            }
        }
    }
    actions: [
        ActionItem {
            id: addButton
            title: "Add"
            imageSource: "asset:///images/ic_add.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (! nameField.text) {
                    nameRequiredLabel.visible = true;
                    return;
                }

                var data = {
                };
                
                data.title = nameField.text; // for T only
                data.name = nameField.text; // for FCGL
                data.note = noteArea.text; // for GT
                data.description = noteArea.text; // for L
                data.private = privateCheckbox.checked; // for FCG
                data.lat = latField.text; // for L
                data.lon = lonField.text; // for L
                data.level = levelDropdown.selectedValue; // for G
                data.contributes = contributesDropdown.selectedValue; // for G

                backingData.add(data);

                listNavPane.pop();
            }
        }
    ]
    attachedObjects: [
        ComponentDefinition {
            // Used to populate drop-down lists
            id: option
            Option {
            }
        }
    ]
    onCreationCompleted: {
    }

    function isF() {
        return "folder" == backingDataType;
    }
    function isT() {
        return "task" == backingDataType;
    }
    function isC() {
        return "context" == backingDataType;
    }
    function isG() {
        return "goal" == backingDataType;
    }
    function isL() {
        return "location" == backingDataType;
    }

    //Called after creating the page to populate fields as required from the passed data
    function setup() {
        populateContributesDropdown();
    }

    // Populates the "Contributes" dropdown for goals to include only higher-level options
    function populateContributesDropdown() {
        // Include "None" Option first
        contributesDropdown.removeAll();
        var noneOpt = option.createObject();
        noneOpt.text = "None";
        noneOpt.value = 0;
        contributesDropdown.add(noneOpt)

        var goals = app.goals.getInternalList();
        // Populate options in contributesDropdown
        for (var i = 0; i < goals.length; i ++) {
            //Don't add archived or higher-level goals to dropdown
            if (goals[i].archived != 0 || goals[i].level >= levelDropdown.selectedValue) {
                continue;
            }
            var opt = option.createObject();
            opt.text = goals[i].name;
            opt.value = goals[i].id;
            contributesDropdown.add(opt);
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

            // name - used by all data types ("title" for tasks)
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                bottomMargin: 30

                Label {
                    id: nameRequiredLabel
                    text: "Required"
                    textStyle.color: Color.Red
                    visible: false
                }
                TextField {
                    id: nameField
                    hintText: backingDataType.charAt(0).toUpperCase() + backingDataType.slice(1) + " Name"
                    horizontalAlignment: HorizontalAlignment.Fill
                    maximumLength: isT() || isG() ? 255 : 32
                }
            }
            // note - used by goals, tasks, location (as "description" with 255-character limit)
            TextArea {
                id: noteArea
                hintText: "Description and notes about this " + backingDataType
                horizontalAlignment: HorizontalAlignment.Fill
                visible: isG() || isT() || isL()
                maximumLength: isL() ? 255 : 32000
                bottomMargin: 30
            }
            // private - used by folders, contexts, goals
            CheckBox {
                id: privateCheckbox
                text: "Private"
                bottomMargin: 30
                visible: isF() || isC() || isG()
            }
            // lat/lon - used by locations
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    visible: isL()

                    TextField {
                        id: latField
                        objectName: "latField"
                        hintText: "Latitude"
                        bottomMargin: 30
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                    }
                    TextField {
                        id: lonField
                        objectName: "lonField"
                        hintText: "Longitude"
                        bottomMargin: 30
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                    }
                }
                Button {
                    id: currentLocationButton
                    objectName: "currentLocationButton"
                    text: "Use Current Location"
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {
                        currentLocationButton.text = "Getting location..."
                        currentLocationButton.enabled = false;
                        app.getLocation();
                    }
                    visible: isL()
                }
            }
            // level - used by goals
            DropDown {
                id: levelDropdown
                title: "Level"
                bottomMargin: 30
                visible: isG()
                options: [
                    Option {
                        text: "Lifetime (default)"
                        value: 0
                    },
                    Option {
                        text: "Long-term"
                        value: 1
                    },
                    Option {
                        text: "Short-term"
                        value: 2
                    }
                ]
                onSelectedValueChanged: {
                    // Rebuild contributesDropDown options to only include higher-level options
                    populateContributesDropdown();
                }
            }
            // contributes - used by goals
            DropDown {
                id: contributesDropdown
                title: "Contributes to"
                visible: isG()
            }
        }
    }
}
