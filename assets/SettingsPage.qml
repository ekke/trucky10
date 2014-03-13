import bb.cascades 1.2

Page {
    id: settingsPage
    property alias conference: conferenceText.text
    titleBar: TitleBar {
        title: "Configuration"
    }
    Container {
        id: outerContainer
        ScrollView {
            id: scrollView
            Container {
                id: contentContainer
                Label {
                    text: "Conference"
                }
                TextField {
                    id: conferenceText
                    hintText: "MobileTechCon 2014"
                    text: "MobileTechCon 2014"
                }
                Divider {
                }
                Label {
                    text: "URI Raspberry Pi"
                }
                TextField {
                    id: arduioneUri
                    hintText: "http://192.168.111.1:92/"
                    text: "http://192.168.111.1:92/"
                    inputMode: TextFieldInputMode.Url
                    onTextChanged: {
                        canbusdata.setUri(text)
                    }
                }
            } // contentContainer
        } // end scrollView
    } // outerContainer
}
