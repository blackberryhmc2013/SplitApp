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
import custom 1.0

Dialog {
    id: taxAndTip
    Container {
        topPadding: 10
        bottomPadding: 10
        leftPadding: 50
        rightPadding: 50
        preferredWidth: 768
        preferredHeight: 400
        background: Color.create(0.0, 0.0, 0.0, 0.5)
        
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        
        Label {
            text: "Tax & Tip Setting"
            horizontalAlignment: HorizontalAlignment.Center
            textStyle.base: SystemDefaults.TextStyles.TitleText
            textStyle.color: Color.White
            topMargin: 50
            bottomMargin: 50
        }
        
        Container {
            bottomMargin: 50
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                Label {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    }
                    text: "Tax: " + Math.floor(tax.value) + "%"
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                }
                
                Slider {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 3.0
                    }
                    id: tax
                    fromValue: 0.0
                    toValue: 30.0
                }
                
                bottomMargin: 10
            }
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                Label {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    }
                    
                    text: "Tip: " + Math.floor(tip.value/5)*5 + "%"
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                }
                
                Slider {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 3.0
                    }
                    id: tip
                    fromValue: 0.0
                    toValue: 30.0
                    
                }
                
                topMargin: 10
            }
        }
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Button {
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                }
                
                text: qsTr("Cancel") + Retranslate.onLanguageChanged
                onClicked: {
                    close();
                }
            }
            
            Button {
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                }
                
                text: qsTr("Submit") + Retranslate.onLanguageChanged
                onClicked: {
                    receiptPage.taxPer = Math.floor(tax.value);
                    receiptPage.tipPer = Math.floor(tip.value);
                    _appUI.setTaxAndTip(Math.floor(tax.value), Math.floor(tip.value/5)*5);
                    close();
                }
            }
        }
    }
}
