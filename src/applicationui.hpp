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

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QUrl>
#include <QtCore/QMetaType>
#include <bb/cascades/Page>
#include <bb/cascades/TouchEvent>
#include <bb/cascades/Container>
#include <bb/cascades/ImageView>
#include <bb/cascades/ActionItem>
#include "HighlightView.h"
#include "Highlight.h"
#include "cloudList/CloudList.h"
#include "NfcShareHandler.hpp"
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include "FileListModel.hpp"
#include <bb/cascades/Label>
#include <QUuid>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <libexif/exif-data.h>
#include <libexif/exif-tag.h>


namespace bb
{
    namespace cascades
    {
        class Application;
        class LocaleHandler;
    }
}

class QTranslator;

/*!
 * @brief Application object
 *
 *
 */

class ApplicationUI : public QObject
{
    Q_OBJECT

public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() { }
    QString id;

public slots:
	void clearAll();
	void setTaxAndTip(double tax, double tip);
	QString getDataBaseId();
	void setupUUID();
	qreal getImageRotation(QUrl _url);

	void changeImgViewSetting();


private slots:
    void onSystemLanguageChanged();
    void onPagePush(bb::cascades::Page* page);
    void onReceiptAreaTouch(bb::cascades::TouchEvent* event);
    void onItemAdded(int index);
    void onItemDeleted(int index);
    void onItemUpdated(int index);
    void onFileAdded(QString str);
    void onInvoked(const bb::system::InvokeRequest& request);
    void recalculateTotal();




private:
    double mTotal;
    double mTax;
    double mTip;
    int idxFromPointer(const Highlight* ptr);
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    QList<int> currentTouchesX;
    QList<int> currentTouchesY;
    bool currTouchInHighlight;
    QList<HighlightView*> highlightViews;
    CloudList<Highlight> highlights;
    QPointer<bb::cascades::Container> receiptView;
    bb::cascades::ActionItem* clearAllButton;
    QString phoneId;
    NfcShareHandler* nfcShareHandler;
    QString filename;
    AbstractPane *root;
    bb::system::InvokeManager* invokeManager;
    bb::system::InvokeRequest request;

    qreal imgRot;
    HighlightView* currentHighlight;

    bb::cascades::ImageView* receiptImgView;
    QUrl imgPath;
    QImage rctImg;
	tesseract::TessBaseAPI mTess;


    QUuid uuid;

    bb::cascades::Label* subTotalLabel;
    bb::cascades::Label* totalLabel;

    QString modelName;
    float yTranslation;
    int actualScreenHeight;


	//buffer
	unsigned char * buffer;
};

#endif /* ApplicationUI_HPP_ */
