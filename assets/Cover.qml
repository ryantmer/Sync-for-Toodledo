import bb.cascades 1.2

Container {
    layout: StackLayout {}
    
    topPadding: 10
    bottomPadding: 10
    leftPadding: 10
    rightPadding: 10
    
    Label {
        text: app.dataModel.data([0]).title
    }
    Label {
        text: dueDateString(app.dataModel.data([0]).duedate)
        textFormat: TextFormat.Auto
    }
    
    function dueDateString(dueDate) {
        if (dueDate == 0) {
            return "No due date";
        } else if (dueDate <= Math.floor((new Date()).getTime() / 1000)) {
            return "<html><body style=\"color:red\"><b>" +
            app.unixTimeToDateTime(dueDate).toDateString() +
            "</b></body></html>";
        } else {
            return app.unixTimeToDateTime(dueDate).toDateString();
        }
    }
}
