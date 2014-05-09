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

#include <QList>
#include <QDir>

#ifndef HIGHLIGHT_H_
#define HIGHLIGHT_H_

class Highlight {
public:
	Highlight(QString stringCode);
	Highlight(double ulx, double uly, double lrx, double lry, QString initPhoneId, double value);
	virtual ~Highlight();

	double getUpperLeftX() const;
	double getUpperLeftY() const;
	double getLowerRightX() const;
	double getLowerRightY() const;
	void setUpperLeft(double x, double y);
	void setLowerRight(double x, double y);

	QList<QString> getPhoneIds() const;
	bool splitBy(QString id) const;

	double getValue() const;
	void setValue(double num);

	QString toString() const;

	void addPhoneId(QString id);
	void removePhoneId(QString id);
	int numIds() const ;

private:
	double upperLeft[2];
	double lowerRight[2];
	// The supposed value of the item highlighted
	double value_;
	// Map of PhoneIds to number of highlights they created
	// List of phoneIds who want to split this item
	QList<QString> phoneIds;

};

#endif

