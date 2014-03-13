import bb.cascades 1.2

NavigationPane {
    id: navigationPane
    
    
    Page {
        id: canbusListPage
        titleBar: TitleBar {
            title: "Last tracked from CAN-Bus"
        }
        actions: [
            ActionItem {
                title: "get data"
                imageSource: "asset:///images/ic_play.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    canbusdata.askForNextData()
                }
            }
        ]
        Container {
            id: outerContainer
            
        } // end outerContainer
        
        
    } // end canbusListPage
}
