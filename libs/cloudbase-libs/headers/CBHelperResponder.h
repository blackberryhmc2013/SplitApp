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
#include "CBHelperAttachment.h"
#include "yajl/YAJLDom.h"


namespace Cloudbase {

#ifndef CBRESPONSEINFO_H_
#define CBRESPONSEINFO_H_

/**
 * A container for the data returned from the cloudbase.io server. This is handed over as a representation
 * of a response to objects implementing the CBHelperResponder interface.
 */
class CBHelperResponseInfo {
public:
	std::string function; /// The function from cloudbase.io (data/notifications/CloudFunction/Applet etc)
	std::string errorMessage; /// An error message, if any, returned by cloudbase.io
	std::string outputString; /// The full String representation of the response from the cloudbase.io servers
	std::string downloadedFileName; /// The full path to the temporary file downloaded by the file download cloudbase.io APIs
	YAJLDom::Value* parsedMessage; /// The parsed JSON message received in the response
	int httpStatusCode; /// The status code for the call
	bool postSuccess; /// Whether the request was successfull
};

#endif

#ifndef CBRESPONDER_H_
#define CBRESPONDER_H_

class CBHelperResponder {
public:
	/**
	* Called when a connection to the cloudbase.io APIs is completed and the data is ready.
	* @param respData A CBHelperResponseInfo object populated with the data from the API request
	* @param result \> 0 on success,
	* or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	*/
	virtual void parseResponse(CBHelperResponseInfo resp) { return; };

	virtual ~CBHelperResponder(void) {}
};

#endif

}
