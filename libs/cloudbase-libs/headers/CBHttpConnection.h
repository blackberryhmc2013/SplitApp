/* Copyright (c) 2013 Cloudbase.io Limited
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
#include <QObject>
#include <QDebug>
#include <QThread>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <QtLocationSubset/QGeoPositionInfo>
#include <QString>
#include <QDir>
#include <QTemporaryFile>


#include "yajl/YAJLDom.h"

#include "CBHelperResponder.h"
#include "CBSerializable.h"
#include "CBHelperAttachment.h"

#define CONNECTION_BUFFER_SIZE 40000
#define REQUEST_PARAMETER_BOUNDARY "---------------------------14737809831466499882746641449"
#define CBHELPER_USER_AGENT "MoSync-CBHelper-0.1a"

namespace Cloudbase {

#ifndef CBHTTPCONNECTION_H_
#define CBHTTPCONNECTION_H_


/**
 * This class is used internally to execute calls to the cloudbase.io APIs. Each request is self-contained within a
 * CBHttpConnection object. This way the CBHelper object can execute multiple requests at the same time.
 */
class CBHttpConnection : public QThread {
	Q_OBJECT
public:
	CBHelperResponder* CBResponder; /// The responder for the response
	std::string authUsername; /// The current username and password for the user interacting with the application - if these security settings are setup
	std::string authPassword;
	std::vector<CBHelperAttachment> atts; /// An array of file attachments for the request
	std::map<std::string, std::string> additionalPostParams; /// Additional post parameters. This is used by the CloudFunctions and Applet APIs
	QtMobilitySubset::QGeoPositionInfo* currentLocation; /// The current location information

	std::string url;
	std::string fileId;
	CBSerializable* parameters;
	CBHelperResponder* responder;

	bool isDownload; /// whether the object has been created to download a file attachment or call the APIs
	bool debugMode;

	/**
	 * Creates a new CBHttpConnection object. Additional parameters may be set in the object after it's been initialised such as the responder,
	 * the Vector of attachments and the eventual additional post parameters
	 * @param appCode The application code
	 * @param appUniq The unique application code
	 * @param password The MD5 of the application password
	 * @param deviceUniq The unique identifier for the device
	 * @param function The cloudbase.io function being called
	 */
	CBHttpConnection(std::string appCode, std::string appUniq, std::string password, std::string deviceUniq, std::string function);

	virtual void run();

	void sendRequest();

	static size_t WriteDataCallback(void *ptr, size_t size, size_t nmemb, void* pInstance) {
		return (static_cast<CBHttpConnection*>(pInstance))->writeData(ptr, size, nmemb);
	}
	static size_t WriteFileDataCallback(void *ptr, size_t size, size_t nmemb, void* pInstance) {
		return (static_cast<CBHttpConnection*>(pInstance))->writeFileData(ptr, size, nmemb);
	}
	size_t writeData(void* ptr, size_t size, size_t nmemb);
	size_t writeFileData(void *ptr, size_t size, size_t nmemb);
protected:
	/**
	 * Parses the response output. Extracts the message data from the JSON and creates the new CBHelperResponseInfo object.
	 * If a CBHelperResponder was given to the class then the parseResponse method is triggered from here once the response
	 * has been validated
	 * @param statusCode The http status code
	 * @param function The cloudbase.io function being called
	 * @param responder The responder object specified for the CBHttpConnection
	 */
	void parseResponseOutput(int statusCode, std::string function);
private:
	FILE *outfile;
	std::string appCode;
	std::string appUniq;
	std::string password;
	std::string function;
	std::string deviceUniqueIdentifier;

	std::string downloadedFilePath;
	std::string responseOutputString;

Q_SIGNALS:
	void requestCompleted(CBHelperResponseInfo resp, CBHelperResponder* responder);
};

#endif /* CBHTTPCONNECTION_H_ */
}
