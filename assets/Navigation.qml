import bb.cascades 1.4

NavigationPane {
    id: navigationPane
    property alias listTitle: listPage.listTitle
    property alias editableContent: listPage.editableContent
    
    ListPage {
        id: listPage
    }
    
    onPopTransitionEnded: {
        page.destroy();
    }
}
