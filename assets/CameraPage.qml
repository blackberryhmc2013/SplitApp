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
    Camera {
        id: camera
        onCreationCompleted: {
            open(CameraUnit.Rear);
        }

        onCameraOpened: {
            getSettings(cameraSettings);
            cameraSettings.focusMode = CameraFocusMode.ContinuousAuto;
            cameraSettings.shootingMode = CameraShootingMode.Stabilization;
            applySettings(cameraSettings);
            startViewfinder();
        }
        
        onTouch: {
            if (event.isDown()) {
                capturePhoto();
            }
        }
        
        onShutterFired: {
            shutterSound.play();
        }
        
        onPhotoCaptured: {
            
        }
        
        onPhotoSaved: {
            camera.stopViewfinder();
            camera.close();
            var page = receiptPageDef.createObject();
            page.receiptImgSrc = fileName;
            _appUI.setupUUID();
            navigationPane.push(page);
        }
        
        attachedObjects: [
            CameraSettings {
                id: cameraSettings
            },
            
            SystemSound {
                id: shutterSound
                sound: SystemSound.CameraShutterEvent
            }
        ]
        
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: receiptPageDef
            source: "ReceiptPage.qml"
        }
    ]
    
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: qsTr("Back") + Retranslate.onLanguageChanged
            onTriggered: {
                navigationPane.pop();
            }
        }
    }
}
