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

#include "Highlight.h"
#include <QStringList>
#include <QDebug>
#include <stdlib.h>

Highlight::Highlight(QString stringCode) {
	QList<QString> tokens = stringCode.split(" ");

	upperLeft[0] = tokens[0].toDouble();
	upperLeft[1] = tokens[1].toDouble();
	lowerRight[0] = tokens[2].toDouble();
	lowerRight[1] = tokens[3].toDouble();
	for (int i = 4; i < tokens.size() - 1; ++i)
		phoneIds.push_back(tokens[i]);
	value_ = tokens.back().toFloat();
}


Highlight::Highlight(double ulx, double uly, double lrx, double lry, QString initPhoneId, double value) : value_(value) {
	upperLeft[0] = ulx;
	upperLeft[1] = uly;
	lowerRight[0] = lrx;
	lowerRight[1] = lry;
	phoneIds.push_back(initPhoneId);
	value_ = value;

}

Highlight::~Highlight(){
	// TODO Auto-generated destructor stub
}

double Highlight::getUpperLeftX() const{
	return upperLeft[0];
}

double Highlight::getUpperLeftY() const{
	return upperLeft[1];
}

double Highlight::getLowerRightX() const{
	return lowerRight[0];
}

double Highlight::getLowerRightY() const{
	return lowerRight[1];
}

double Highlight::getValue() const {
	return value_;
}
/*
QString Highlight::getPhoneId() const {
	return phoneId;
}
*/


bool Highlight::splitBy(QString id) const {
	return phoneIds.contains(id);
}

int Highlight::numIds() const {
	return phoneIds.length();
}

QList<QString> Highlight::getPhoneIds() const{
	return phoneIds;
}

QString Highlight::toString() const{
	QString string = "";
	for(int i=0; i<phoneIds.size(); i++) {
		string += phoneIds[i];
		if(i<phoneIds.size()-1)
			string += "," ;
	}

	QString stringCode = QString::number(upperLeft[0]) + " " + QString::number(upperLeft[1])
	                     + " " + QString::number(lowerRight[0]) + " " +
	                     QString::number(lowerRight[1]) + " " + string
	                     + " " + QString::number(value_);
    return stringCode;
}


void Highlight::setUpperLeft(double x, double y){
	upperLeft[0] = x;
	upperLeft[1] = y;

}

void Highlight::setLowerRight(double x, double y){
	lowerRight[0] = x;
	lowerRight[1] = y;
}

void Highlight::setValue(double value){
	value_ = value;
}


void Highlight::addPhoneId(QString id){
	phoneIds.push_back(id);
}



void Highlight::removePhoneId(QString id){
    phoneIds.removeAll(id);
}

