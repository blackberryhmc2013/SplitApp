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
#include <QtLocationSubset/QGeoCoordinate>

#include "../yajl/YAJLDom.h"

namespace Cloudbase {

#ifndef CBGEOLOCATEDOBJECT_H_
#define CBGEOLOCATEDOBJECT_H_

/**
 * Represents an object returned by a CBGeoDataStream with its
 * coordinates, altitude and additional information stored in the
 * cloud database collection
 */
class CBGeoLocatedObject : public QObject {
public:

	CBGeoLocatedObject();

	/**
	 * The coordinate position of the object
	 */
	QtMobilitySubset::QGeoCoordinate coordinate;
	/**
	 * The altitude of the object if the cb_location_altitude field
	 * exists in the document
	 */
	double altitude;
	/**
	 * All of the other data stored in the cloud database for the
	 * document
	 */
	YAJLDom::Value* objectData;

	int hash();
};

#endif

}
