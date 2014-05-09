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

#include "FileListModel.hpp"
#include <QString>

FileListModel::FileListModel(QObject *parent)
{
    setParent(parent);
}

QList<QUrl> FileListModel::files() const
{
    QList<QUrl> fileList;
    for (int row = 0; row < size(); ++row)
        fileList << QUrl::fromLocalFile(value(row));

    return fileList;
}

void FileListModel::addFile(const QString &filePath, QString uuid)
{
    clear();
	const int pos = indexOf(filePath);
    if (pos != -1)
        return;

    QString newPath = filePath;
    int number = newPath.indexOf(".jpg") - newPath.indexOf("IMG");
    newPath.replace(newPath.indexOf("IMG"), number, uuid);

    QFile::copy(filePath, newPath);

    append(newPath);
    bb::system::InvokeManager iManager;
    bb::system::InvokeRequest request;
	// Who do we want to send the invoke request to?
	request.setTarget("com.split.view");
	// What do we want the target application to do with it?
	request.setAction("bb.action.VIEW");
	// What are we sending?
	request.setMimeType("image/jpeg");
	// Where is the data?
	request.setUri(QUrl("file://" +filePath));
	request.setData("");
	bb::system::InvokeTargetReply *reply = iManager.invoke(request);
	// Remember to take ownership of the reply object.
	reply->setParent(this);
	// Listen for the invoke response.

	// If any Q_ASSERT statement(s) indicate that the slot failed to connect to
	// the signal, make sure you know exactly why this has happened. This is not
	// normal, and will cause your app to stop working!!
	bool connectResult;

	// Since the variable is not used in the app, this is added to avoid a
	// compiler warning.
	Q_UNUSED(connectResult);

	connectResult =  QObject::connect(reply, SIGNAL(finished()),
									  this,  SLOT(onInvokeResult()));

	// This is only available in Debug builds.
	 Q_ASSERT(connectResult);

	// Store reply somewhere, so you can
	// access it when onInvokeResult fires.
	_invokeTargetReply = reply;
    emit changed();
}

void FileListModel::removeFile(const QString &filePath)
{
    const int pos = indexOf(filePath);
    if (pos == -1)
        return;

    removeAt(pos);
    emit changed();
}

// Because we connected this method to the finished() SIGNAL on the
// reply object, this method will be called when the application
// receives an invoke response.
void FileListModel::onInvokeResult()
{
    // Check for errors
    switch(_invokeTargetReply->error()) {
        // Invocation could not find the target
        // did we use the right target ID?
    case bb::system::InvokeReplyError::NoTarget: {
            qDebug() << "invokeFinished(): Error: no target";
            break;
        }
        // There was a problem with the invoke request
        // did we set all the values correctly?
    case bb::system::InvokeReplyError::BadRequest: {
    	qDebug() << "invokeFinished(): Error: bad request";
            break;
        }
        // Something went completely
        // wrong inside the invocation request
        // Find an alternate route :(
    case bb::system::InvokeReplyError::Internal: {
    	qDebug() << "invokeFinished(): Error: internal";
            break;
        }
        //Message received if the invoke request is successful
    default:
    	qDebug() << "invokeFinished(): Invoke Succeeded";
        break;
    }

    // A little house keeping never hurts...
    delete _invokeTargetReply;
}

