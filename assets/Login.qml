import bb.cascades 1.2
import bb.system 1.2

Page {
    id: loginPage;
    objectName: "loginPage"
    accessibility.name: "Login page"
    
    Container {
        layout: DockLayout {}
        
        ScrollView {
            id: loginScrollView
            objectName: "loginScrollView"
            scrollViewProperties.pinchToZoomEnabled: false
            scrollViewProperties.scrollMode: ScrollMode.Vertical
            accessibility.name: "Login page ScrollView"
            
            WebView {
                id: loginWebView
                objectName: "loginWebView"
                accessibility.name: "Login page WebView"
                
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
            accessibility.name: "Container for progress indicator"
            bottomPadding: 25
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            
            ProgressIndicator {
                id: progressIndicator
                opacity: 0
                accessibility.name: "Progress indicator for loading login page"
            }
        }
    }
}