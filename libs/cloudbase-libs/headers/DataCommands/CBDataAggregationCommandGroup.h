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
#include "CBDataAggregationCommand.h"

namespace Cloudbase {

#ifndef CBHELPERDATACOMMANDGROUP_H
#define CBHELPERDATACOMMANDGROUP_H

typedef enum {
	CBDataAggregationGroupSum,
	CBDataAggregationGroupMax,
	CBDataAggregationGroupMin,
	CBDataAggregationGroupAvg
} CBDataAggregationGroupOperator;

/**
 * The group aggregation command. This works exaclty in the same way a GROUP BY
 * command would work in SQL.
 * The outputField array contains the number of fields for the output to be
 * "grouped by".
 * There's a number of operators to apply to the grouped field defined as
 * CBDataAggregationGroupOperator
 */
class CBDataAggregationCommandGroup : public CBDataAggregationCommand {
private:
	std::vector<std::string> idFields;
	std::vector<std::string> groupFields;
public:
	const static std::string CBDataAggregationGroupOperator_ToString[];

	CBDataAggregationCommandGroup();
	virtual ~CBDataAggregationCommandGroup();

	/**
	 * Adds a field to the list of fields the output should be
	 * grouped by
	 * @param fieldName An NSString with the name of the field
	 */
	void addOutputField(std::string fieldName);

	/**
	 * Adds a calculated field to the output of this group clause using a static value
	 * @param outputField The name of the output field
	 * @param op The operator to apply to the selected variable field
	 * @param groupValue A value to be used with the operator
	 */
	void addGroupFormulaForValue(std::string outputField, CBDataAggregationGroupOperator op, std::string groupValue);
	/**
	 * Adds a calculated field to the output of this group clause using the value of another field
	 * @param outputField The name of the output field
	 * @param op The operator to apply to the selected variable field
	 * @param fieldName The name of the variable field to be used with the operator
	 */
	void addGroupFormulaForField(std::string outputField, CBDataAggregationGroupOperator op, std::string fieldName);

	std::string serializeAggregateConditions();
};

#endif

}
