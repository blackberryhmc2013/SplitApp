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

//#include "CBHelper.h"

namespace Cloudbase {

#ifndef CBHELPERSEARCHCONDITION_H
#define CBHELPERSEARCHCONDITION_H
/**
 * The possible operators for a search on the CloudBase database
 */
typedef enum CBConditionOperator {
	CBOperatorEqual,           //!< CBOperatorEqual
    CBOperatorLess,         //!< CBOperatorLess
    CBOperatorLessOrEqual,  //!< CBOperatorLessOrEqual
    CBOperatorBigger,       //!< CBOperatorBigger
    CBOperatorBiggerOrEqual,//!< CBOperatorBiggerOrEqual
    CBOperatorAll,          //!< CBOperatorAll
    CBOperatorExists,       //!< CBOperatorExists
    CBOperatorMod,          //!< CBOperatorMod
    CBOperatorNe,           //!< CBOperatorNe
    CBOperatorIn,           //!< CBOperatorIn
    CBOperatorNin,          //!< CBOperatorNin
    CBOperatorSize,         //!< CBOperatorSize
    CBOperatorType,         //!< CBOperatorType
    CBOperatorWithin,       //!< CBOperatorWithin
    CBOperatorNear          //!< CBOperatorNear
} CBConditionOperator;

/**
 * Conditions to concatenate multiple conditions
 */
typedef enum CBConditionLink {
	CBConditionLinkAnd,//!< CBConditionLinkAnd
	CBConditionLinkOr, //!< CBConditionLinkOr
	CBConditionLinkNor //!< CBConditionLinkNor
} CBConditionLink;

/**
 * Direction for sorting of search results
 */
typedef enum CBSortDirection {
	CBSortAscending, //!< CBSortAscending
	CBSortDescending //!< CBSortDescending
} CBSortDirection;

/**
 * This class represents a set of conditions to run a search over your CloudBase - this is the equivalent
 * representation of a WHERE clause in SQL.
 * Each condition can have a number of sub-conditions and each one is linked to the previous using a CBConditionLink
 */
class CBHelperSearchCondition : public CBDataAggregationCommand, public CBSerializable {
public:
	/// std::string arrays to convert the condition operator and link from the struct to a std::string
	const static std::string CBConditionOperator_ToString[];
	const static std::string CBConditionLink_ToString[];
	const static std::string CBSortDirection_ToString[];

	/**
	 * Creates a new empty search conditions. Before being used this condition will need to set at
	 * least a field, value and operator
	 */
	CBHelperSearchCondition();
	/**
	 * Creates a new condition to run a basic search over a field.
	 * @param field The name of the field on the CloudBase collection
	 * @param value The value to look for. It is possible to use regular expressions in
	 * this field. For more information see the CloudBase documentation on the REST APIs
	 * page: http://cloudbase.io/documentation/rest-apis#CloudBase
	 * @param op The operator for the condition
	 */
	CBHelperSearchCondition(std::string field, std::string value, CBConditionOperator op);
	/**
	 * Creates a new location-based condition. This will run over the default cb_location field
	 * set on the documents when location information are sent.
	 * This performs a search within a boundary box specified with the 2 corners
	 * @param SWLat The latitude for the south-western corner of the boundary box
	 * @param SWLng The longitude for the south-western corner of the boundary box
	 * @param NELat The latitude for the north-eastern corner of the boundary box
	 * @param NELng The longitude for the north-eastern corner of the boundary box
	 */
	CBHelperSearchCondition(double SWLat, double SWLng, double NELat, double NELng);
	/**
	 * Creates a new location-based condition. This will run over the default cb_location field
	 * set on the documents when location information are sent.
	 * This performs a search around the given location.
	 * @param lat The latitude of the point we are looking for
	 * @param lng The longitude of the point to search around
	 * @param maxDistance The size in meters of the area to consider around the given point
	 */
	CBHelperSearchCondition(double lat, double lng, double maxDistance);

	/**
	 * Sets the link between the current condition and the previous one
	 * @param link The CBConditionLink element
	 */
	void setConditionLink(CBConditionLink link);
	/**
	 * Adds a condition to the list of subconditions for the current one.
	 * @param newCond The new condition to be added
	 */
	void addCondition(CBHelperSearchCondition* newCond);
	/**
	 * Add a sorting condition to your search. You can add multiple fields to sort by.
	 * It is only possible to sort on top level fields and not on objects.
	 * @param fieldName The name of the field in the collection
	 * @param dir A CBSortDirection value
	 */
	void addSortField(std::string fieldName, CBSortDirection dir);

	// internal methods used to serialize a CBCondition object and its sub-conditions.
	virtual std::string serializeAggregateConditions();
	virtual std::string serialize();
	std::string serialize(CBHelperSearchCondition* cond, bool isTop = true);

	std::string field_;
	std::string value_;
	int conditionOperator_;
	int contidionLink_;
	std::vector<CBHelperSearchCondition*> conditions_;
	std::vector<std::string> sortFields_;
	/**
	 * This property is the maximum number of results to be returned by the search
	 */
	int limit;
	int offset;
protected:
	void baseInit();

};

#endif

}
