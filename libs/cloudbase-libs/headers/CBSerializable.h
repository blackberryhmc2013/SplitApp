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

#ifndef CBSERIALIZABLE_H_
#define CBSERIALIZABLE_H_

namespace Cloudbase {

/**
 * This interface should be implemented by all objects that need to be sent and stored in the CloudBase.
 * The serialize() method is called by the CBHelper object when the document needs to be sent to the
 * cloudbase.io REST APIs
 */
class CBSerializable {
public:
	/**
	 * This method should produce a String containing the JSON representation of the current object.
	 * The cloudbase.io APIs are capable of inserting multiple objects in one call and expect the structure
	 * passed to be an array, even if just one object is present.
	 * Therefore this method should always return a String beginning with "[":
	 * [ { "first_name" : "cloud", "last_name" : "base", "title" : ".io" } ]
	 * @return The JSON representation of the current object
	 */
	virtual std::string serialize() { return "[ {} ]"; };

	virtual ~CBSerializable(void) {}
};

}

#endif
