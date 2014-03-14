import bb.cascades 1.2
import bb.system 1.2

Page {
    id: canbusListPage
    titleBar: TitleBar {
        title: "Last tracked from CAN-Bus"
    }
    attachedObjects: [
        SystemToast {
            id: toastInfo
            property bool waiting: false
            body: "Getting data from CAN Bus Interface ..."
            icon: "asset:///images/icon.png"
            onFinished: {
                if(waiting){
                    toastInfo.show()
                }
            }
        }
    ]
    actions: [
        ActionItem {
            title: "get data"
            imageSource: "asset:///images/ic_play.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                canbusdata.askForNextData()
                toastInfo.show()
                toastInfo.waiting = true
            }
        },
        DeleteActionItem {
            title: "clear List"
            onTriggered: {
                dataModelCanbus.clear()
            }
        }
    ]
    Container {
        id: outerContainer
        ListView {
            id: canbusList
            dataModel: GroupDataModel {
                id: dataModelCanbus
                sortingKeys: [ "day", "timeString" ]
                grouping: ItemGrouping.ByFullValue
            } // end dataModel
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    CustomListItem {
                        id: itemRoot
                        dividerVisible: true
                        highlightAppearance: HighlightAppearance.Full
                        Container {
                            id: outerItemContainer
                            leftPadding: 20
                            rightPadding: 20
                            verticalAlignment: VerticalAlignment.Center
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            Container {
                                id: imageContainer
                                leftPadding: 20
                                rightPadding: 20
                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 0.5
                                }
                                ImageView {
                                    imageSource: "asset:///images/" + ListItemData.imageName + ".png"
                                    verticalAlignment: VerticalAlignment.Center
                                    horizontalAlignment: HorizontalAlignment.Center
                                    scalingMethod: ScalingMethod.AspectFit
                                    minHeight: 114
                                    maxHeight: 114
                                }
                            } // imageContainer
                            Container {
                                id: rowsContainer
                                layout: StackLayout {
                                    orientation: LayoutOrientation.TopToBottom
                                }
                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 2.0
                                }
                                Container {
                                    id: firstRowContainer
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    Label {
                                        text: ListItemData.sensorName
                                        textStyle.base: SystemDefaults.TextStyles.TitleText
                                        verticalAlignment: VerticalAlignment.Center
                                        layoutProperties: StackLayoutProperties {
                                            spaceQuota: 2.0
                                        }
                                    } // end sensorName
                                    Label {
                                        text: ListItemData.sensorValue
                                        textStyle.base: SystemDefaults.TextStyles.TitleText
                                        verticalAlignment: VerticalAlignment.Center
                                        layoutProperties: StackLayoutProperties {
                                            spaceQuota: 1.0
                                        }
                                    } // end sensorValue
                                } // firstRowContainer
                                Container {
                                    id: secondRowContainer
                                    Label {
                                        text: ListItemData.timestamp
                                        textStyle.base: SystemDefaults.TextStyles.BodyText
                                        textStyle.textAlign: TextAlign.Right
                                        textStyle.color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.LightGray : Color.DarkGray
                                        verticalAlignment: VerticalAlignment.Center
                                    } // end timestamp
                                } // secondRowContainer
                                Container {
                                    id: thirdRowContainer
                                    bottomPadding: 10
                                    Label {
                                        text: ListItemData.oid
                                        textStyle.fontSize: FontSize.XSmall
                                        textStyle.textAlign: TextAlign.Right
                                        textStyle.color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Dark ? Color.LightGray : Color.DarkGray
                                        verticalAlignment: VerticalAlignment.Center
                                    } // end oid
                                } // secondRowContainer
                            } // end rowsContainer
                        } // outerItemContainer
                    } // CustomListItem itemRoot
                }, // end item
                ListItemComponent {
                    type: "header"
                    CustomListItem {
                        id: headerRoot
                        dividerVisible: true
                        highlightAppearance: HighlightAppearance.None
                        Container {
                            leftPadding: 16
                            topPadding: 10
                            rightPadding: 10
                            verticalAlignment: VerticalAlignment.Center
                            Label {
                                text: ListItemData
                                textStyle.base: SystemDefaults.TextStyles.TitleText
                                textStyle.color: Color.Magenta
                                verticalAlignment: VerticalAlignment.Center
                            }
                        }
                    } // CustomListItem headerRoot
                } // end header
            ] // end listItemComponents
            onTriggered: {
                //
            }
        } // end canbusList
    } // end outerContainer
    function disconnect() {
        //
    }
    function onResponseData(data) {
        dataModelCanbus.insertList(data)
        toastInfo.waiting = false
        toastInfo.cancel()
    }
    onCreationCompleted: {
        canbusdata.responseData.connect(onResponseData)
    }
}
