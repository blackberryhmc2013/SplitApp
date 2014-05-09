/* Copyright (c) 2013-2014 BlackBerry Limited.
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
import bb.cascades.multimedia 1.0
import bb.multimedia 1.0
import custom 1.0


Page {
    id: receiptPage
    objectName: "receiptPage"
    property alias recfile: viewimage.imageSource
    property alias zAngle: viewimage.rotationZ
    property string receiptImgSrc
    property double taxPer
    property double tipPer
    
    Container {
    	layout: StackLayout {
         orientation: LayoutOrientation.TopToBottom
        }
    	
        Container {

            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            topPadding: 30
            leftPadding: 30
            rightPadding: 30
            bottomPadding: 10

            Label {
                id: subTotal
                objectName: "subTotal"
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                text: "SubTotal: $0"
            }
            
            Label {
                objectName: "Total"
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                text: "Total: $0"
            }
        }
        
        Divider {
            
        }
        
		Container {
		    objectName: "receiptView"
            layout: AbsoluteLayout {
            }

            ImageView {
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                objectName: "receiptImage"
                id: viewimage
                imageSource: "file://" + receiptImgSrc
                rotationZ: _appUI.getImageRotation("file://" + receiptImgSrc);
                scalingMethod: ScalingMethod.AspectFit
                preferredHeight: layoutHandler.layoutFrame.height
                preferredWidth: layoutHandler.layoutFrame.width
                attachedObjects: [
                    LayoutUpdateHandler {
                        id: handler
                        onLayoutFrameChanged: {
							viewimage.preferredHeight = layoutFrame.height
							viewimage.preferredWidth = layoutFrame.width
                        }
                    }
                ]
            }

        }
               
        attachedObjects: LayoutUpdateHandler {
            id: layoutHandler
        }
    }
    
    paneProperties: NavigationPaneProperties {
        id: navigationBar
        backButton: ActionItem {
            title: qsTr("Back") + Retranslate.onLanguageChanged
            onTriggered: {
                _nfcShareHandler.shareMode = NfcShareHandler.DataShare
                _nfcShareHandler.fileModel.removeFile(receiptImgSrc)
                navigationPane.pop();
                camera.open(CameraUnit.Rear);
                _appUI.clearAll();
            }
        }
    }
    
    actions: [
        ActionItem {
            title: qsTr("Share") + Retranslate.onLanguageChanged
            imageSource: "asset:///buttons/share.png"
            
            onTriggered: {
                _nfcShareHandler.shareMode = NfcShareHandler.FileShare
                _nfcShareHandler.fileModel.addFile(receiptImgSrc, _appUI.getDataBaseId())
            }
        },
        
        ActionItem {
            objectName: "clearAllButton"
            title: qsTr("Clear All") + Retranslate.onLanguageChanged
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///buttons/clear.png"
            
            onTriggered: {
            }
        },
        
        ActionItem {
            title: qsTr("Tax & Tip") + Retranslate.onLanguageChanged
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///buttons/tips.png"
            
            onTriggered: {
                taxAndTip.open();
            }
        }
    ]
    
    attachedObjects: [
        TaxTipDialog {
            id: taxAndTip
        }
    ]
}
