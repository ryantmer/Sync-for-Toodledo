import bb.cascades 1.4

NavigationPane {
    id: navigationPane
    property alias listTitle: listPage.listTitle
    property alias listPage: listPage
    
    ListPage {
        id: listPage
    }
    
    onPopTransitionEnded: {
        page.destroy();
    }
}
