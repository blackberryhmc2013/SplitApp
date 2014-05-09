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
#ifndef CLOUDLIST_PRIVATE_H_
#define CLOUDLIST_PRIVATE_H_

// Private functions

template <typename T>
void CloudList<T>::updateInternal(QString id, QString data, qulonglong stamp, bool deleted ) {
	typename Container::iterator it = elements.begin();
	int i = 0;
	for(; it < elements.end(); ++it, ++i) {
		if( id.compare(it->dbId_) == 0) {
			it->deleted_ = deleted;
			it->stamp_ = QDateTime::fromMSecsSinceEpoch(stamp);

			if(deleted) {
				qDebug() << "delete: " << i;
				// This SHOULD be blocking
				emit deletedObject(i);
				elements.removeAt(i);
			} else {
				it->data_ = T(data);
				emit updatedObject(i);
			}
			qDebug() << "updated existing item w stamp" ;
			return;
		}
	}

	if (!deleted) {
		elements.push_back(CloudList<T>::Element(T(data), id, QDateTime::fromMSecsSinceEpoch(stamp)));
		emit newObject(elements.size() - 1);
	}
}

template <typename T>
void CloudList<T>::updateCloud(int index) {
	UpdateResponder* cb = new UpdateResponder();
	qDebug() << "want to update: " << elements.at(index).dbId_;
 	Cloudbase::CBHelperSearchCondition* searchCond =
			new Cloudbase::CBHelperSearchCondition("cb_id", elements.at(index).dbId_.toString().toStdString(),
					(CBConditionOperator)0);  // sketchy... need to fix this
	helper->updateDocument(databaseName.toStdString(), &elements[index], searchCond, cb);
}

template <typename T>
void CloudList<T>::pullData() {
	PullResponder* resp = new PullResponder(this);
	Cloudbase::CBHelperSearchCondition* searchCond =
				new Cloudbase::CBHelperSearchCondition("deleted", "0",
						(CBConditionOperator)0);  // sketchy... need to fix this
	helper->searchDocument(databaseName.toStdString(), searchCond, resp);
}

template <typename T>
void CloudList<T>::createSessionCompleted(const bb::network::PushStatus& status) {
        if (!status.isError() && pushService) {
        	qDebug() << "Session creation completed successfully";
    		pushService->registerToLaunch();
        } else {
            qDebug() << "Session creation failed: " << status.errorDescription();
    		pushService->registerToLaunch();
        }
}

template <typename T>
void CloudList<T>::onRegisterToLaunchCompleted(const bb::network::PushStatus& status) {
	qDebug("onRegisterToLaunchCompleted");
	if (pushService->hasConnection()){
		pushService -> createChannel(QUrl("http://cp4418.pushapi.eval.blackberry.com"));
	} else {
		qDebug("No connection");
	}
}

template <typename T>
void CloudList<T>::createChannelCompleted(const bb::network::PushStatus& status, const QString& channel) {
	if (!status.isError() && pushService) {
		qDebug() << "Channel creation completed successfully " << channel;
	} else {
		qDebug() << "Channel creation failed: " + status.errorDescription();
	}
}

template <typename T>
void CloudList<T>::onInvoked(const bb::system::InvokeRequest& request) {
	qDebug() << "GOT PUSH";
	//Check whether the app was invoked via a push
	if (request.action().compare("bb.action.PUSH") != 0) {
		qDebug() << "Not a push invocation :("; //Nope, so we return
		return;
	}
	//If the app is here we know the InvokeRequest contains push data
	bb::network::PushPayload payload(request);
	if (payload.isValid()) {
		if (payload.isAckRequired()) { //This section is only useful for Push Plus
			pushService->acceptPush(payload.id());
		}
		//Read all the push data from the payload
		QString message = payload.data();
		//Pass it to our method which will store the push in a nice format
		qDebug() << "GOT PUSH: " << message;

		//For the purposes of this sample only we are expecting that all push data
		// is formatted into 4 parts using a '|' character as a delimiter.
		QStringList messageParts = message.split('|');
		if(messageParts.at(0).compare(databaseName) == 0)
			updateInternal(messageParts.at(1), messageParts.at(3), messageParts.at(4).toULongLong(),
					messageParts.at(2).compare("delete") == 0);
		qDebug() << "internals updated";
	}
}


#endif /* CLOUDLIST_PRIVATE_H_ */
