/* Copyright (C) 2011 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/*
 * YAJLDom.h
 *
 *  Created on: Jan 3, 2011
 *  Author: Niklas Nummelin
 */
#include <string>
#include <map>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <QtGlobal>

#include "api/yajl_parse.h"
#include "api/yajl_gen.h"

#include "Stack.h"

#ifndef _YAJL_DOM_H_
#define _YAJL_DOM_H_

namespace YAJLDom {

#define newobject( type, obj ) ( obj )
#define deleteobject( obj )\
	{\
		if ( (obj) != NULL )\
		{\
			delete (obj);\
			(obj) = NULL;\
		}\
	}
struct KeyString {
	KeyString() {}
	KeyString(const char* s, int len) : str(s), length(len) {
	}
	const char* str;
	int length;
};


class Value {
	public:
		enum Type {
			NUL,
			BOOLEAN,
			NUMBER,
			ARRAY,
			MAP,
			STRING
		};

		Value(Type type);
		virtual ~Value();

		Type getType() const;
		bool isNull() const;

		virtual std::string toString() const = 0;
		virtual bool toBoolean() const;
		virtual int toInt() const;
		virtual double toDouble() const;
		virtual Value* getValueForKey(const std::string& key);
		virtual Value* getValueByIndex(int i);

		virtual const Value* getValueForKey(const std::string& key) const;
		virtual  const Value* getValueByIndex(int i) const;

		virtual int getNumChildValues() const;

	private:
		Type mType;

	};

	class NullValue : public Value {
	public:
		NullValue();
		std::string toString() const;
	};

	class BooleanValue : public Value {
	public:
		BooleanValue(bool value);
		std::string toString() const;
		bool toBoolean() const;
		void setBoolean(bool value);

	private:
		bool mValue;
	};

	class NumberValue : public Value {
	public:
		NumberValue(double num);
		std::string toString() const;
		int toInt() const;
		double toDouble() const;

	private:
		double mValue;
	};

	class StringValue : public Value {
	public:
		StringValue(const char* str, size_t length);
		StringValue(const std::string& str);
		std::string toString() const;
	private:
		std::string mValue;
	};

	class MapValue : public Value {
	public:
		MapValue();
		~MapValue();

		void setValueForKey(const std::string& key, Value* value);
		Value* getValueForKey(const std::string& key);
		const Value* getValueForKey(const std::string& key) const;

		std::string toString() const;

	private:
		std::map<std::string, Value*> mMap;
	};

	class ArrayValue : public Value {
	public:
		ArrayValue();
		~ArrayValue();

		void addValue(Value* value);

		Value* getValueByIndex(int i);
		const Value* getValueByIndex(int i) const;
		int getNumChildValues() const;

		const std::vector<Value*>& getValues() const;

		std::string toString() const;
	private:
		std::vector<Value*> mValues;
	};

	class YAJLParser {
	public:
		YAJLParser() {};

		Value *sRoot;
		Stack<Value*> sValueStack;
		Stack<KeyString> sKeyStack;
		yajl_gen g;

		/**
		 * Parse Json string data and return the root node of
		 * the document tree.
		 * \param jsonText UTF8 or ASCII.
		 * \param jsonTextLength Length of Json text.
		 * \return The root node if successful, or NULL on error.
		 * The returned node must be deallocated with delete.
		 */
		Value* parse(const unsigned char* jsonText, size_t jsonTextLength);

		/**
		 * Use this function to safely delete a value (won't do anything if the value is NULL or equal to sNullValue).
		 * sNullValue might be returned if you do getValueByIndex or getValueForKey and the key or element doesn't exist.
		 */
		void deleteValue(Value* value);

		// static methods
		static int parse_null(void * ctx);
		static int parse_boolean(void * ctx, int boolean);
		static int parse_number(void * ctx, const char * s, unsigned int l);
		static int parse_string(void * ctx, const unsigned char * stringVal, unsigned int stringLen);
		static int parse_map_key(void * ctx, const unsigned char * stringVal, unsigned int stringLen);
		static int parse_start_map(void * ctx);
		static int parse_end_map(void * ctx);
		static int parse_start_array(void * ctx);
		static int parse_end_array(void * ctx);

		Value* validateValue(Value* value, Value::Type type);
		void printValue(Value* value);
		void pushValue(Value *value);
		void popValue();
		void gen_print(void *ctx, const char *str, unsigned int len);
		void parseError(yajl_handle hand, int verbose, const unsigned char* jsonText, size_t jsonTextLength);
	};

} // namespace YAJLDom


#endif // _YAJL_DOM_H_
