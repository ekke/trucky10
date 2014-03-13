import bb.cascades 1.2

Page {
    
    Container {
        ImageView {
            imageSource: "asset:///images/truck_720.png"
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top
            scalingMethod: ScalingMethod.AspectFill
            
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
            topPadding: 200
            Label {
                text: "Welcome Trucky"
                textStyle.fontSize: FontSize.XLarge
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                text: rootPane.conference
                textStyle.fontSize: FontSize.XLarge
                horizontalAlignment: HorizontalAlignment.Center
            }
        }
        
    }
}
