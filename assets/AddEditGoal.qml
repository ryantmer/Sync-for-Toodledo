import bb.cascades 1.2

Page {
    id: addEditGoalPage
    
    property variant data;
    property bool edit;
    
    function setup() {
        var goals = app.goalDataModel.getInternalList();
        
        //Populate options in contributes dropdown
        for (var x = 0; x < goals.length; x++) {
            //If editing, don't add current option or higher-level ones to contributesDropDown
            if (edit) {
                if (goals[x].id == data.id || goals[x].level >= data.level) {
                    continue;
                }
            }
            //Don't add archived or higher-level goals to dropdown
            if (goals[x].archived != 0 || goals[x].level >= levelDropDown.selectedValue) {
                continue;
            }
            var opt = option.createObject();
            opt.text = goals[x].name;
            opt.value = goals[x].id;
            contributesDropDown.add(opt);
        }
        
        if (edit) {
            var index;
            
            //Populate the fields of the form as required
            nameField.text = data.name;
            for (index = 0; index <= levelDropDown.options.length; index++) {
                if (data.level == levelDropDown.options[index].value) {
                    break;
                }
            }
            levelDropDown.setSelectedIndex(index);
            for (index = 0; index < contributesDropDown.options.length; index++) {
                if (data.contributes == contributesDropDown.options[index].value) {
                    break;
                }
            }
            contributesDropDown.setSelectedIndex(index);
            archivedCheckBox.checked = data.archived;
            privateCheckBox.checked = data.private;
            
            addSaveButton.title = "Save";
            addSaveButton.imageSource = "asset:///images/ic_save.png";
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
            title: "Add" //Changed to "Save" in setup if editing a goal
            imageSource: "asset:///images/ic_add.png" //Changed in setup if editing a goal
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (!nameField.text) {
                    titleRequired.visible = true;
                    return;
                }
                if (nameField.text.length > 32) {
                    nameTooLong.visible = true;
                    return;
                }
    
                var newData = {}
                if (edit) {
                    newData = data;
                }
                newData["name"] = nameField.text;
                newData["level"] = levelDropDown.selectedValue;
                newData["contributes"] = contributesDropDown.selectedValue;
                newData["archived"] = archivedCheckBox.checked + 0;
                newData["private"] = privateCheckBox.checked + 0;
                newData["note"] = noteArea.text;
                
//                for (var param in newData) {
//                    console.log("newData." + param + " = " + newData[param]);
//                }
                
                if (edit) {
                    app.goalDataModel.edit(data, newData);
                } else {
                    app.goalDataModel.add(newData);
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
            
            Label {
                id: titleRequired
                text: "Required"
                textStyle.color: Color.Red
                visible: false
            }
            Label {
                id: nameTooLong
                text: "Max 32 characters"
                textStyle.color: Color.Red
                visible: false
            }
            TextField {
                id: nameField
                hintText: "Goal Name"
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 30
            }
            DropDown {
                id: levelDropDown
                title: "Level"
                bottomMargin: 30
                options: [
                    Option {
                        text: "Lifetime"
                        value: 0
                        selected: true //overwritten by setup() if we're editing
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
                    //Rebuild contributesDropDown options to only include higher-level options
                    contributesDropDown.removeAll();
                    var noneOpt = option.createObject();
                    noneOpt.text = "None";
                    noneOpt.value = 0;
                    contributesDropDown.add(noneOpt)
                    var goals = app.goalDataModel.getInternalList();
                    for (var x = 0; x < goals.length; x++) {
                        //If editing, don't add current option or higher-level ones to contributesDropDown
                        if (edit) {
                            if (goals[x].id == data.id || goals[x].level >= data.level) {
                                continue;
                            }
                        }
                        //Don't add archived or higher-level goals to dropdown
                        if (goals[x].archived != 0 || goals[x].level >= levelDropDown.selectedValue) {
                            continue;
                        }
                        var opt = option.createObject();
                        opt.text = goals[x].name;
                        opt.value = goals[x].id;
                        contributesDropDown.add(opt);
                    }
                }
            }
            DropDown {
                id: contributesDropDown
                title: "Contributes to"
                Option {
                    text: "None"
                    value: 0
                    selected: true
                }
            }
            CheckBox {
                id: archivedCheckBox
                text: "Archived"
                bottomMargin: 30
            }
            CheckBox {
                id: privateCheckBox
                text: "Private"
                bottomMargin: 30
            }
            TextArea {
                id: noteArea
                hintText: "Detailed notes about goal"
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 30
            }
        }
    }
    
    attachedObjects: [
        ComponentDefinition {
            //Used to populate drop-downs
            id: option
            Option {}
        }
    ]
}
