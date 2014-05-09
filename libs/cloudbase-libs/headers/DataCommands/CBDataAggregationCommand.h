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
#include <string>

namespace Cloudbase {

#ifndef CBHELPERDATAAGGDOMMAND_H_
#define CBHELPERDATAAGGDOMMAND_H_

typedef enum {
	CBDataAggregationProject,
	CBDataAggregationUnwind,
	CBDataAggregationGroup,
	CBDataAggregationMatch
} CBDataAggregationCommandType;

/**
 * This class should be implemented by any class to send
 * Data Aggregation commands to cloudbase.io
 *
 * The serializeAggregateConditions should resturn a Dictionary
 * exactly in the format needed by the CBHelper class to be added
 * to the list of parmeters, serliazed and sent to cloudbase.io
 */
class CBDataAggregationCommand {
protected:
	/**
	 * This property should be set by each sub-class and it represent which
	 * kind of Aggregation operation this object is.
	 * Without this paramter the object cannot be sent to cloudbase.io
	 */
	CBDataAggregationCommandType commandType;
public:
	const static std::string CBDataAggregationCommandType_ToString[];
	/**
	 * Transforms the current command type to a string for the purpose
	 * of serialization.
	 *
	 * @return The NSString to be sent to cloudbase.io
	 */
	std::string getCommandTypeString();

	virtual ~CBDataAggregationCommand();

	/**
	 * Serializes the Command object to its JSON representation
	 *
	 * @return A NSDictionary representation of the Command object. This
	 *  method should be implemented in each subclass
	 */
	virtual std::string serializeAggregateConditions() { return " { } "; };
};
#endif
}
