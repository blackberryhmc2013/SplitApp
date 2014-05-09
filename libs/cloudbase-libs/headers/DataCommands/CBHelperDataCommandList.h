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
#include <vector>
#include "../CBSerializable.h"
#include "CBDataAggregationCommand.h"

namespace Cloudbase {

#ifndef CBHELPERDATACOMMANDLIST_H
#define CBHELPERDATACOMMANDLIST_H

/**
 * This object represents an ordered list of CBDataAggregationCommand
 * objects. It is used for the commands to be serialized and handed
 * to a request to the cloudbase.io APIs
 */
class CBHelperDataCommandList : public CBSerializable {
private:
	std::vector<CBDataAggregationCommand*> commands;
public:
	/**
	 * Creates a new instance of the list
	 */
	CBHelperDataCommandList();

	/**
	 * Adds a command to the list. This is an ordered list and commands
	 * should be added exactly in the order they are meant to be executed
	 * in.
	 * @param command The new command to be added to the list
	 */
	void addCommand(CBDataAggregationCommand* command);

	virtual std::string serialize();
};

#endif

}
