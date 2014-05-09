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
#include <QTimer>
#include <QtLocationSubset/QGeoPositionInfo>
#include <QtLocationSubset/QGeoCoordinate>

#include "../CBHelper.h"
#include "../DataCommands/CBHelperSearchCondition.h"

#include "CBGeoLocatedObject.h"

namespace Cloudbase {

#ifndef CBGEODATASTREAM_H_
#define CBGEODATASTREAM_H_

#define CBGEODATASTREAM_UPDATE_SPEED    1000
#define CBGEODATASTREAM_RADIUS_RATIO    4.0

class CBGeoDataStream : public QObject, CBHelperResponder {
	Q_OBJECT
public:
	/**
	 * The collection on which to run the search
	 */
	std::string collection;
	/**
	 * The radius for the next search in meters from the point returned by the
	 * getLatestPosition method
	 */
	double searchRadius;
	/**
	 * The name given in its constructor to this CBGeoDataStream
	 */
	std::string streamName;

	/**
	 * Creates a new CBGeoDataStream object using the given helper class to query the
	 * cloudbase.io APIs.
	 *
	 * @param helper An initialized CBHelper object
	 * @param collection The name of the collection containing the geospatial data in the cloud database
	 * @param searchRadius The initial search radius in meters.
	 */
	CBGeoDataStream(std::string streamName, CBHelper* helper, std::string collection, double searchRadius);

	/**
	 * Begins querying the cloudbase.io APIs and returning data periodically.
	 */
	void startStream();

	/**
	 * Stops the data stream
	 */
	void stopStream();

	virtual void parseResponse(Cloudbase::CBHelperResponseInfo resp);
private:
	CBHelper* helper;
	QtMobilitySubset::QGeoPositionInfo* previousPosition;
	QTimer *timer;
	double previousSpeed;
	double queryRadius;
	std::map<int, CBGeoLocatedObject*> foundObjects;
	bool isRunning;

Q_SIGNALS:
	/**
	 * Returns the latest known position to the CBGeoDataStream object.
	 * This is used to retrieve the data and compute the movement speed to
	 * increase or decrease the speed of refresh
	 *
	 * @return A valid QtMobilitySubset::QGeoPositionInfo* pointer
	 */
	QtMobilitySubset::QGeoPositionInfo* getLatestPosition(std::string streamName);
	/**
	 * receives a new point to be visualized
	 *
	 * @param CBGeoLocatedObject An object representing a new point on the map
	 */
	void receivedPoint(CBGeoLocatedObject* newPoint, std::string streamName);
	/**
	 * Informs the application that the CBGeoDataStream is removing a point from its cache
	 *
	 * @param CBGeoLocatedObject The point being removed
	 */
	void removingPoint(CBGeoLocatedObject* point, std::string streamName);

public Q_SLOTS:
	void retrievePoints();
};

#endif /* CBGEODATASTREAM_H_ */

}
