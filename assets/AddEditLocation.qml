import bb.cascades 1.2

Page {
    id: addEditLocationPage
    
    property variant data;
    property bool edit;
    
    function setup() {
        if (edit) {
            nameField.text = data.name;
            descriptionArea.text = data.description;
            latField.text = data.lat;
            lonField.text = data.lon;
            
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
            title: "Add" //Changed to "Save" in setup if editing a location
            imageSource: "asset:///images/ic_add.png" //Changed in setup if editing a location
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
                newData["description"] = descriptionArea.text;
                newData["lat"] = latField.text;
                newData["lon"] = lonField.text;
                
//                for (var param in newData) {
//                    console.log("newData." + param + " = " + newData[param]);
//                }
                
                if (edit) {
                    app.locationDataModel.edit(data, newData);
                } else {
                    app.locationDataModel.add(newData);
                }
                mainNavPane.pop();
            }
        }
    ]
    
    ScrollView {
        accessibility.name: "Add/edit location scrollview"
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
                hintText: "Location Name"
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 30
            }
            TextArea {
                id: descriptionArea
                hintText: "Location description"
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 30
            }
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
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
            }
        }
    }
}
