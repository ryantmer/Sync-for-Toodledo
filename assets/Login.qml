import bb.cascades 1.2
import bb.system 1.2

Page {
    id: loginPage;
    ScrollView {
        scrollViewProperties.pinchToZoomEnabled: false
        scrollViewProperties.scrollMode: ScrollMode.Vertical
        
        WebView {
            id: loginWebView
            objectName: "loginWebView"
            url: "https://api.toodledo.com/3/account/authorize.php?response_type=code&client_id=ToodleDo10&state=random_string_lololol&scope=basic%20tasks%20notes"
            //TODO: Update to include all permissions, including write
            
            onUrlChanged: {
                console.log(url);
            }
        }
    }
}