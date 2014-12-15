import bb.cascades 1.2
import bb.system 1.2

Page {
    id: loginPage;
    objectName: "loginPage"
    
    Container {
        layout: DockLayout {}
        
        ScrollView {
            id: loginScrollView
            objectName: "loginScrollView"
            accessibility.name: "Login scrollview"
            scrollViewProperties.pinchToZoomEnabled: false
            scrollViewProperties.scrollMode: ScrollMode.Vertical
            
            WebView {
                id: loginWebView
                objectName: "loginWebView"
                accessibility.name: "Login webview"
                
                onLoadProgressChanged: {
                    progressIndicator.value = loadProgress / 100.0;
                }
                
                onLoadingChanged: {
                    if (loadRequest.status == WebLoadStatus.Started) {
                        progressIndicator.opacity = 1.0;
                    } else if (loadRequest.status == WebLoadStatus.Succeeded) {
                        progressIndicator.opacity = 0.0;
                    }
                }
            }
        }
        
        Container {
            bottomPadding: 25
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            
            ProgressIndicator {
                id: progressIndicator
                accessibility.name: "Login page loading progress indicator"
                opacity: 0
            }
        }
    }
}