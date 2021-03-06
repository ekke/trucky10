/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.2

TabbedPane {
    id: rootPane
    property string conference: settingsPage.conference
    showTabsOnActionBar: true
    
    Tab {
        id: homeTab
        title: "Home"
        imageSource: "asset:///images/home.png"
        HomePage {
            id: homePage
        }
    } // end homeTab
    
    Tab {
        id: canbusTab
        title: "Can-bus Data"
        imageSource: "asset:///images/ic_view_list.png"
        CanbusPane {
            id: canbusPane
        }
    }
    
    Tab {
        id: settingsTab
        title: "Config"
        SettingsPage {
            id: settingsPage
        }
    
    } // settingsTab
    
    onActiveTabChanged: {
        if(activeTab == homeTab){
            showTabsOnActionBar = true
        } else {
            showTabsOnActionBar = false
        }
    }
    
    onCreationCompleted: {
        //
    }
} // end rootPane
