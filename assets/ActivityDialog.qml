import bb.cascades 1.2

Dialog {
    id: activityDialog
    
    Container {
        layout: DockLayout {}
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        background: Color.create(1, 1, 1, 0.6)
        topPadding: 20
        bottomPadding: topPadding
        leftPadding: topPadding
        rightPadding: topPadding
        
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
            topPadding: 20
            bottomPadding: topPadding
            leftPadding: topPadding
            rightPadding: topPadding
            
            ActivityIndicator {
                visible: true
                running: true
                preferredHeight: 100
            }
            Label {
                id: activityText
                objectName: "activityText"
                text: "Loading..."
                verticalAlignment: VerticalAlignment.Center
            }
        }
    }
}