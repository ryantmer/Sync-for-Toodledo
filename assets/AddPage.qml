import bb.cascades 1.4

Page {
    id: addTaskPage

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
                if (! titleField.text) {
                    titleRequiredLabel.visible = true;
                    return;
                }

                var data = {
                };

                data.title = titleField.text;
                data.note = noteArea.text;

                app.data.addItem("task", data);

                listNavPane.pop();
            }
        }
    ]

    function setup() {
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
        }
    }
}
