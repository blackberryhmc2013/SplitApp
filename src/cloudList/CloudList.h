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

#ifndef CLOUDLIST_H_
#define CLOUDLIST_H_

#include "CloudListConfig.h"

#include <vector>
#include "libs/cloudbase-libs/headers/CBHelper.h"
#include <bb/data/JsonDataAccess>
#include <sstream>
#include <bb/network/PushService>
#include <bb/network/PushStatus>
#include <bb/network/PushPayload>
#include <bb/system/InvokeManager>
#include "SimplePushServer.hpp"
#include <QUuid>

using Cloudbase::CBConditionOperator;

class CloudListSignals : public QObject
{
	Q_OBJECT

public:
	CloudListSignals(QObject *parent = 0) { Q_UNUSED(parent) };

signals:
	void newObject(int index);
	void updatedObject(int index);
	void deletedObject(int index);
public slots:
	virtual void pullData() = 0;
	virtual void createChannelCompleted(const bb::network::PushStatus& status, const QString& channel) = 0;
	virtual void onInvoked(const bb::system::InvokeRequest& request) = 0;
	virtual void createSessionCompleted(const bb::network::PushStatus& status) = 0;
	virtual void onRegisterToLaunchCompleted(const bb::network::PushStatus& status) = 0;
};

template <typename T>
class CloudList : public CloudListSignals {

public:
	CloudList(QString dbName);
	virtual ~CloudList();
	void add(T data);
	int size() const;
	const T& at(int index) const;
	T& operator[] (int index);
	void remove(int index);
	void update(int index);
	const QDateTime lastUpdate(int index) const;
	QString databaseName;

private:
	// Wrapper for objects
	class Element : public Cloudbase::CBSerializable
	{
	public:
		Element(T data, QUuid id = QUuid::createUuid() , QDateTime stamp = QDateTime::currentDateTime());
		//Element(std::string JSON);
		bool deleted_;
		QDateTime stamp_;
		T data_;
		QUuid dbId_;
		// The repr function
		std::string serialize();
		T& get();
		const T& getConst() const;
	};
	typedef QList<Element> Container;
	Container elements;

	// Internal updates of cloud changed data
	void updateInternal(QString id, QString data, qulonglong stamp, bool deleted);
	void updateCloud(int index);
	void pullData();
	void createSessionCompleted(const bb::network::PushStatus&);
	void onRegisterToLaunchCompleted(const bb::network::PushStatus& status);
	void createChannelCompleted(const bb::network::PushStatus& status, const QString& channel);
	void onInvoked(const bb::system::InvokeRequest& request);

	// CloudBase things
	Cloudbase::CBHelper* helper;
	bb::network::PushService* pushService;
	bb::system::InvokeManager* invokeManager;

	SimplePushServer* pushServer;

    class InsertResponder : public Cloudbase::CBHelperResponder
    {
    public:
    	InsertResponder(Element* ele, SimplePushServer* pushServer) : ele_(ele), pushServer_(pushServer) { } ;
    	Element* ele_;
    	SimplePushServer* pushServer_;
    	void parseResponse(Cloudbase::CBHelperResponseInfo resp);
    };

    class UpdateResponder : public Cloudbase::CBHelperResponder
    {
    public:
    	UpdateResponder() { };
    	void parseResponse(Cloudbase::CBHelperResponseInfo resp);
    };

    class PullResponder : public Cloudbase::CBHelperResponder
    {
    public:
    	PullResponder(CloudList<T>* parent) : parent(parent) { };
    	CloudList<T>* parent;
    	void parseResponse(Cloudbase::CBHelperResponseInfo resp);
    };
};

template <typename T>
CloudList<T>::CloudList(QString dbName) : databaseName(dbName) {
	 helper = new Cloudbase::CBHelper(CLOUDLIST_APP_CODE, CLOUDLIST_APP_UNIQ);
	 helper->setPassword(CLOUDLIST_APP_MD5);
	 helper->logEvent("Connected from cloudlist","General");

	 pushService = new bb::network::PushService(BLACKBERRY_PUSH_APP_ID,
			 BLACKBERRY_INVOKE_TARGET_ID);

	 invokeManager = new bb::system::InvokeManager(this);
	 connect(invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), SLOT(onInvoked(const bb::system::InvokeRequest&)));

     connect(pushService,SIGNAL(createSessionCompleted (const bb::network::PushStatus &)),
             this, SLOT(createSessionCompleted(const bb::network::PushStatus&)));

     connect(pushService, SIGNAL(registerToLaunchCompleted(const bb::network::PushStatus&)),
             this, SLOT(onRegisterToLaunchCompleted(const bb::network::PushStatus&)));

	 connect(pushService,SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString&)),
			 this, SLOT(createChannelCompleted(const bb::network::PushStatus&, const QString&)));

	 pushService->createSession();
	 pushServer = new SimplePushServer();

	 pullData();
}

template <typename T>
CloudList<T>::~CloudList() {
	//pushService->destroyChannel();
}

template <>
inline void CloudList<QString>::add(QString data) {
	elements.push_back(Element(data));
	InsertResponder* cb = new InsertResponder(&elements.back(), pushServer);
	helper->insertDocument(databaseName.toStdString(), &elements.back(), cb);
	pushServer -> pushMessageToUserList(databaseName, elements.back().dbId_, "add",
			elements.back().data_, elements.back().stamp_.toMSecsSinceEpoch());
	emit newObject(elements.size()-1);
}

template <typename T>
void CloudList<T>::add(T data) {
	elements.push_back(Element(data));
	InsertResponder* cb = new InsertResponder(&elements.back(), pushServer);
	helper->insertDocument(databaseName.toStdString(), &elements.back(), cb);
	pushServer -> pushMessageToUserList( databaseName, elements.back().dbId_,
			"add",  elements.back().data_.toString(), elements.back().stamp_.toMSecsSinceEpoch());
	emit newObject(elements.size()-1);
}

template <typename T>
const T& CloudList<T>::at(int index) const {
	return elements.at(index).getConst();
}

template <typename T>
T& CloudList<T>::operator[] (int index) {
 	return elements[index].get();
 }


template <typename T>
const QDateTime CloudList<T>::lastUpdate(int index) const {
	return elements.at(index).stamp_;
}

template <typename T>
void CloudList<T>::remove(int index) {
	elements[index].deleted_ = true;
	elements[index].stamp_ = QDateTime::currentDateTime();
	updateCloud(index);
	pushServer->pushMessageToUserList(databaseName, elements.at(index).dbId_, "delete" , "",
			elements.at(index).stamp_.toMSecsSinceEpoch());

	// could be optimized away..
	updateInternal(elements.at(index).dbId_, "", elements.at(index).stamp_.toMSecsSinceEpoch(),
			elements.at(index).deleted_);
}

// Push updates to the cloud and push recipients
template <typename T>
void CloudList<T>::update(int index) {
	elements[index].stamp_ = QDateTime::currentDateTime();
	updateCloud(index);
	pushServer->pushMessageToUserList(databaseName, elements.at(index).dbId_, "update" ,
			elements.back().data_.toString(), elements.at(index).stamp_.toMSecsSinceEpoch());
	emit updatedObject(index);
}

// Push updates to the cloud and push recipients
template <>
inline void CloudList<QString>::update(int index) {
	elements[index].stamp_ = QDateTime::currentDateTime();
	updateCloud(index);
	pushServer->pushMessageToUserList(databaseName, elements.at(index).dbId_, "update" ,
			elements.back().data_, elements.at(index).stamp_.toMSecsSinceEpoch());
	emit updatedObject(index);
}


template <typename T>
int CloudList<T>::size() const {
	return elements.size();
}

#include "CloudList_private.h"
#include "CloudList_elements.h"
#include "CloudList_responders.h"


#endif /* CLOUDLIST_H_ */
