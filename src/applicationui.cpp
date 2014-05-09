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

#include "applicationui.hpp"

#include <bb/cascades/Container>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/TouchEvent>
#include <bb/device/HardwareInfo>
//#include <bb/cascades/pickers/FilePicker>
#include <bb/cascades/pickers/FileType>
#include <bb/system/ApplicationStartupMode>
#include <bb/cascades/ScalingMethod>
#include <QLocale>
#include <QTranslator>
#include <bb/device/DisplayInfo>
#include <bb/ImageData>

#include <bb/PixelFormat>
#include <QLocale>
#include <QTranslator>

#include <QListIterator>

#include <stdlib.h>

using namespace bb::cascades;
using namespace std;


ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app), nfcShareHandler(new NfcShareHandler()),
        highlights(QUuid().createUuid().toString()), currentHighlight(NULL)
{
	// find current phone's id
	phoneId = bb::device::HardwareInfo().pin();

    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    // set image name to empty string
    filename = "";

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    //QObject::connect(&invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),this, SLOT(onInvoke(const bb::system::InvokeRequest&)));

    qmlRegisterUncreatableType<NfcShareHandler>("custom", 1, 0, "NfcShareHandler", "Access to enums");
    qmlRegisterUncreatableType<bb::cascades::pickers::FileType>("custom", 1, 0, "FileType", "Access to enums");
    qmlRegisterType<FileListModel>();
    QmlDocument* qml;

    invokeManager = new bb::system::InvokeManager(this);
    bool invokedByShare = false;
    switch(invokeManager -> startupMode()) {
		case bb::system::ApplicationStartupMode::LaunchApplication:
			// An application can initialize if it
			// was launched from the home screen
			qml = QmlDocument::create("asset:///main.qml").parent(this);
			qml->setContextProperty("_nfcShareHandler", nfcShareHandler);
			break;

		case bb::system::ApplicationStartupMode::InvokeApplication:
			// If the application is invoked,
			// it must wait until it receives an invoked(..) signal
			// so that it can determine the UI that it needs to initialize
			qml = QmlDocument::create("asset:///ReceiptPage.qml").parent(this);
			invokedByShare = true;
			break;
		default:
			// What app is it and how did it get here?
			break;
    }


    qml->setContextProperty("_appUI", this);
    // initial load
    onSystemLanguageChanged();

    // Create root object for the UI
    root = qml->createRootObject<AbstractPane>();

   	connect(invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
   			          this, SLOT(onInvoked(const bb::system::InvokeRequest&)));

    connect(root, SIGNAL(pushTransitionEnded(bb::cascades::Page*)),
     	 	this, SLOT(onPagePush(bb::cascades::Page*)));
    connect(&highlights, SIGNAL(newObject(int)), this, SLOT(onItemAdded(int)));
    connect(&highlights, SIGNAL(deletedObject(int)), this, SLOT(onItemDeleted(int)));
    connect(&highlights, SIGNAL(updatedObject(int)), this, SLOT(onItemUpdated(int)));

    // set total, tax and tip to 0
    mTotal = 0;
    mTax = 0;
    mTip = 0;

    // Set up Tesseract
    QString appFolder(QDir::homePath());
    QString pathTemp = appFolder + "/../app/native/assets/";
    QByteArray tempBtArray = pathTemp.toUtf8();
    const char * TESSDATA = tempBtArray.constData();

    modelName = bb::device::HardwareInfo().modelName();
    actualScreenHeight = bb::device::DisplayInfo().pixelSize().height();

    setenv("TESSDATA_PREFIX", TESSDATA, 1);

    if(mTess.Init(TESSDATA, "eng", tesseract::OEM_TESSERACT_ONLY)){
	   qDebug() << ">>>>>>>>>>>>>>>>>>>  tesseract init error";
    }

    // Set created root object as the application scene
    app->setScene(root);

    if (invokedByShare) {
    	onPagePush((bb::cascades::Page*)root);
    }
}


void ApplicationUI::changeImgViewSetting(){
	if (modelName == "Z10" || modelName == "Z30"){
		receiptImgView->setScaleX(1.4);
		receiptImgView->setScaleY(1.4);
		receiptImgView->setTranslationY(-125.0);
		yTranslation = -125.0;
		receiptImgView->setScalingMethod(ScalingMethod::AspectFit);
	}

	if (modelName == "Q10"){
		receiptImgView->setScaleX(0.9);
		receiptImgView->setScaleY(0.9);
		receiptImgView->setTranslationY(-90);
		yTranslation = -90;
		receiptImgView->setScalingMethod(ScalingMethod::AspectFit);
	}

	receiptImgView->setImageSource(imgPath);
}


void ApplicationUI::setupUUID(){
	// create uuid and set it as highlights' database name
	uuid = QUuid().createUuid();
	id = uuid.toString();
	id = id.mid(1, id.length()-2);
	highlights.databaseName = id;
}

QString ApplicationUI::getDataBaseId(){
	return id;
}

void ApplicationUI::onItemAdded(int index) {

	if(highlights.at(index).splitBy(phoneId)) {
		return;
	}

	if(!receiptView.isNull()) {

		float screenWidth = receiptImgView->preferredWidth();
		float screenHeight = receiptImgView->preferredHeight();
		float imageWidth = rctImg.width();
		float imageHeight = rctImg.height();

		int topContainerSize = actualScreenHeight - screenHeight;

		int imgInScreenWidth = imageWidth/imageHeight * screenHeight;
		int leftMargin = 0;

		if (modelName == "Q10"){
			leftMargin = (screenWidth-imgInScreenWidth)/2;
		}

		int xMin = highlights.at(index).getUpperLeftX() * screenWidth - leftMargin;
		int xMax = highlights.at(index).getLowerRightX()* screenWidth - leftMargin;
		int yMin = highlights.at(index).getUpperLeftY() * screenHeight
				  - topContainerSize;
		int yMax = highlights.at(index).getLowerRightY()* screenHeight
				  - topContainerSize;

		HighlightView* view = new HighlightView(xMax - xMin, yMax - yMin,xMin,yMin, HighlightView::othersColor,
				false, &highlights[index]);
		highlightViews.push_back(view);
		receiptView -> add(view);
	}
	recalculateTotal();
}

void ApplicationUI::onItemDeleted(int index) {

	// find which highlightView points to the delete index
	QList<HighlightView*>::Iterator it = highlightViews.begin();
	for(; it < highlightViews.end(); ++it) {
		if ( idxFromPointer((*it)->m_datPtr) == index) {
			receiptView->remove(*it);
			highlightViews.erase(it);
			return;
		}
	}
	recalculateTotal();

//
//	receiptView->remove(highlightViews.at(index));
//	highlightViews.removeAt(index);
}

void ApplicationUI::onItemUpdated(int index) {
	recalculateTotal();
}

void ApplicationUI::clearAll() {
	// This is made more complicated because we are removing as we loop..

	QListIterator<HighlightView*> iter(highlightViews);
	//iter.toBack();

	while(iter.hasNext()){
		HighlightView * temp = iter.next();

		if (temp->m_datPtr->getPhoneIds().contains(phoneId)){
			temp->m_datPtr->removePhoneId(phoneId);
			if (temp->m_datPtr->numIds() == 0){
				highlights.remove(idxFromPointer(temp->m_datPtr));
			}

			else{
				temp->showAsOthers();
			}
		}
	}

	recalculateTotal();
}


void ApplicationUI::setTaxAndTip(double tax, double tip){
	mTax = tax;
	mTip = tip;
	recalculateTotal();
}



qreal ApplicationUI::getImageRotation(QUrl _url) {
	qreal rotation = -1;
	QString url = _url.toLocalFile();
	if (url.endsWith("jpg", Qt::CaseInsensitive) || url.endsWith("tiff", Qt::CaseInsensitive)) {
		// hold onto this so constData() is valid
		QByteArray ba = url.toLocal8Bit();
		ExifData* exifData = exif_data_new_from_file(ba.constData());
		if (exifData != NULL) {
			ExifEntry* exifEntry = exif_content_get_entry(exifData->ifd[EXIF_IFD_0],
					EXIF_TAG_ORIENTATION);

			if (exifEntry != NULL) {
				char value[256] = { 0, };
				memset(value, 0, sizeof(value));
				exif_entry_get_value(exifEntry, value, sizeof(value));

				QString orient = QString::fromLocal8Bit(value);
				if (orient.compare("bottom-right", Qt::CaseInsensitive) == 0) {
					rotation = 180.0;
				} else if (orient.compare("right-top", Qt::CaseInsensitive) == 0) {
					rotation = 90.0;
				} else if (orient.compare("left-bottom", Qt::CaseInsensitive) == 0) {
					rotation = 270.0;
				}

				delete exifEntry;
			}
			delete exifData;
		}
	}
	imgRot = rotation;
	return rotation;
}

void ApplicationUI::onPagePush(bb::cascades::Page* page)
{
	// If the receipt page has been shown its time for us to get started
	if(page->objectName() == "receiptPage") {
		receiptView = page->findChild<Container*>("receiptView");
		clearAllButton = page->findChild<ActionItem*>("clearAllButton");

		receiptImgView = page->findChild<ImageView*>("receiptImage");
		subTotalLabel = page->findChild<Label*>("subTotal");
		totalLabel = page->findChild<Label*>("Total");


		imgPath = receiptImgView->imageSource();

		QString imgPathString = imgPath.toEncoded();
		int length = imgPathString.length() - 7;
		QString cutImgPath = imgPathString.right(length);
		bool loaded = rctImg.load(cutImgPath);

		QTransform rot;
		rot.rotate(imgRot);
		rctImg = rctImg.transformed(rot);

		changeImgViewSetting();

		connect(receiptView, SIGNAL(touch(bb::cascades::TouchEvent*)),
				this, SLOT(onReceiptAreaTouch(bb::cascades::TouchEvent*)));

		connect(clearAllButton, SIGNAL(triggered()),
						this, SLOT(clearAll()));
	}
}


void ApplicationUI::recalculateTotal(){

	mTotal = 0;

	for(int i = 0; i < highlightViews.size(); i++){
		HighlightView* view = highlightViews.at(i);
		Highlight* tmpPtr = (Highlight*)view->m_datPtr;

		if(tmpPtr->numIds()!= 0 && tmpPtr->getPhoneIds().contains(phoneId)){

			int myShare = 0;

			for(int j = 0; j < tmpPtr->numIds(); j++){
				if (tmpPtr->getPhoneIds().at(j) == phoneId){
					myShare++;
				}
			}
			mTotal += tmpPtr->getValue()/tmpPtr->numIds()*myShare;
		}
	}

	QString tempSubTotal = "SubTotal: $" + QString::number(mTotal, 'f', 2);
	subTotalLabel->setText(tempSubTotal);

	double tTotal = mTotal + mTotal*(mTax/100) + mTotal*(mTip/100);
	QString tempTotal = "Total: $" + QString::number(tTotal, 'f', 2);
	totalLabel->setText(tempTotal);
}


void ApplicationUI::onReceiptAreaTouch(bb::cascades::TouchEvent* event)
{
	if (event->isUp()) {
		for (int i = highlightViews.size() - 1; i >= 0; --i) {
			HighlightView* view = highlightViews.at(i);
			if(view->isInsideArea(event->localX(), event->localY())) {
				currTouchInHighlight = true;
				Highlight* tmpPtr = view->m_datPtr;
				view->showAsMine();

				tmpPtr->addPhoneId(phoneId);
				highlights.update(i);
			}
		}
		if(!currTouchInHighlight) {
			if(receiptView) {
				//Fix coordinates
				float screenWidth = receiptImgView->preferredWidth();
				float screenHeight = receiptImgView->preferredHeight();

				// Get dimensions of the container and the real image
				// do the computations and hope it will work
				float imageWidth = rctImg.width();
				float imageHeight = rctImg.height();

				float widthScale = imageWidth/screenWidth;
				float heightScale = imageHeight/screenHeight;

				int xMax = currentHighlight->botRightX();
				int xMin = currentHighlight->topLeftX();
				int yMax = currentHighlight->botRightY();
				int yMin = currentHighlight->topLeftY();

				int bufferWidth = (xMax - xMin) * widthScale;
				int bufferHeight = (yMax - yMin) * heightScale;

				int xImg = (xMin)*widthScale;
				int yImg = (yMin-yTranslation)*heightScale;


				unsigned char * buf = (unsigned char*) malloc((bufferWidth + 1) * bufferHeight);
				for (int y = 0; y < bufferHeight; y++){
					xImg = (xMin)*widthScale;
					for (int x = 0; x < bufferWidth; x++){

						QRgb srcPixel = rctImg.pixel(xImg, yImg);
						int xOffset = bufferWidth * y;
						int yOffset = x;
						int offset = xOffset + yOffset;
						buf[offset] = qGray(srcPixel);
						xImg++;

					}
					yImg++;
				}

				mTess.SetImage(buf, bufferWidth, bufferHeight, 1, bufferWidth);
				mTess.SetRectangle(0, 0, bufferWidth, bufferHeight);
				char * outText = mTess.GetUTF8Text();
                double value = atof(outText);


                delete buf;

                Highlight highlight(1.0*xMin/screenWidth,1.0*(yMin-yTranslation)/screenHeight,
						1.0*xMax/screenWidth, 1.0*(yMax-yTranslation)/screenHeight,phoneId,value);


				highlights.add(highlight);
				// making highlights non movable for now
				currentHighlight->m_datPtr = &highlights[highlights.size() - 1];
				highlightViews.push_back(currentHighlight);
			}
			recalculateTotal();
			currentHighlight = NULL;
		}
	} else if (event->isMove()) {
		if(!currentHighlight) {
			for (int i = highlightViews.size() - 1; i >= 0; --i) {
				HighlightView* view = highlightViews.at(i);
				if(view->isInsideArea(event->localX(), event->localY())) {
					currTouchInHighlight = true;
					break;
				}
			}

			if(!currTouchInHighlight) {
				currentHighlight =  new HighlightView(50,50,event->localX(), event->localY(), HighlightView::myColor, false, NULL);
				receiptView -> add(currentHighlight);
			}
		} else {
			currentHighlight->setBotRight(event->localX(), event->localY());
		}
	} else {
		currTouchInHighlight = false;
	}
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("SplitUI_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

// Find the index of a highlight from its pointer.  Should be replaced
// with iterator erase
int ApplicationUI::idxFromPointer(const Highlight* ptr)
{
	for(int i = 0; i < highlights.size() - 1; ++i) {
		if (&highlights.at(i) == ptr)
			return i;
	}
}

void ApplicationUI::onInvoked(const bb::system::InvokeRequest& request)
{
	if (request.action().compare("bb.action.OPEN") != 0) {
		return;
	}

	 modelName = bb::device::HardwareInfo().modelName();

	QString filename = request.uri().toEncoded();
	QStringList fileinParts = filename.split('/'); //split filename to manipulate
	QString str = fileinParts.join("/");
	root->setProperty("recfile", filename);
	imgPath = str;

	QString imgPathString = imgPath.toEncoded();

	qreal angle = getImageRotation(str);
	root->setProperty("zAngle", angle);

	int length = imgPathString.length() - 7;
	QString cutImgPath = imgPathString.right(length);
	bool loaded = rctImg.load(cutImgPath);

	QTransform rot;
	rot.rotate(imgRot);

	rctImg = rctImg.transformed(rot);


	int n = filename.lastIndexOf("/");
	int m = filename.length() - n - 1;
	id = filename.right(m).left(m-4);


	highlights.databaseName = id;
}

void ApplicationUI::onFileAdded(QString str) {

}
