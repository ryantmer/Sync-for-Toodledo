import bb.cascades 1.4

Page {
    property variant data
    property string backingDataType

    function setup() {

    }
    Container {
        Label {
            text: "Congratulations on for edit yay " + backingDataType
        }
    }
}