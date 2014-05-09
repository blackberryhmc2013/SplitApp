/*
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
#ifndef CLOUDLIST_ELEMENTS_H_
#define CLOUDLIST_ELEMENTS_H_

/* ELEMENT THINGS */

template <typename T>
CloudList<T>::Element::Element(T data, QUuid id, QDateTime stamp)
 : deleted_(false), stamp_(stamp), data_(data), dbId_(id)
{

}

template <typename T>
const T& CloudList<T>::Element::getConst() const
{
	return data_;
}

template <typename T>
T& CloudList<T>::Element::get()
{
	return data_;
}

template <typename T>
std::string CloudList<T>::Element::serialize() {
    std::stringstream out;
    QString data = data_.toString();

    out << "[ {";
    out << "\"data\" : ";
    // If the representation is a JSON array then don't quotate it
    if(data[0] == '{')
    	out << data.normalized(QString::NormalizationForm_D).toStdString() << ", "; // make things ascii bescause the backend can't handle it!
    else
    	out << "\"" << data.toStdString() << "\", ";
    out << "\"stamp\" : \"" << stamp_.toMSecsSinceEpoch() << "\", ";
    out << "\"deleted\" : \"" << deleted_ << "\" ";

  	out << ", \"id\" : \"" << dbId_.toString().toStdString() << "\" ";

    out << "} ] ";
    return out.str();
}

template <>
inline std::string CloudList<QString>::Element::serialize() {
    std::stringstream out;
    out << "[ {" << "\"data\" : \"" << data_.toStdString() << "\", "
    		     << "\"stamp\" : \"" << stamp_.toMSecsSinceEpoch() << "\", "
    		     << "\"deleted\" : \"" << deleted_ << "\" ";

    out << ", \"id\" : \"" << dbId_.toString().toStdString() << "\" ";

	out << "} ] ";
	return out.str();
}


#endif /* CLOUDLIST_ELEMENTS_H_ */
