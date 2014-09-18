import bb.cascades 1.2
import bb.system 1.2

Page {
    id: loginPage;
    objectName: "loginPage"
    ScrollView {
        id: loginScrollView
        objectName: "loginScrollView"
        scrollViewProperties.pinchToZoomEnabled: false
        scrollViewProperties.scrollMode: ScrollMode.Vertical
        accessibility.name: "Login Page ScrollView"
        
        WebView {
            id: loginWebView
            objectName: "loginWebView"
            accessibility.name: "Login Page WebView"
        }
    }
}