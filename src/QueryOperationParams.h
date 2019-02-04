//---------------------------------------------------------------------------------------------------------------------------------
// File: OdbcOperation.h
// Contents: ODBC Operation objects called on background thread
// 
// Copyright Microsoft Corporation and contributors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//
// You may obtain a copy of the License at:
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//---------------------------------------------------------------------------------------------------------------------------------

#pragma once

#include <stdafx.h>

namespace mssql
{
	using namespace std;
	using namespace v8;

	class QueryOperationParams
	{
	public:

		wstring query_string() { return _query_string; }
		int64_t id() { return _id; }
		int32_t timeout() { return _timeout; }
		int32_t query_tz_adjustment() { return _query_tz_adjustment; }
		bool polling() { return _polling; }

		QueryOperationParams(Handle<Number> query_id, Handle<Object> query_object)
		{
			auto qs = get(query_object, "query_str");
			nodeTypeFactory fact;
			auto context = fact.isolate->GetCurrentContext();
			auto maybe = qs->ToString(context);
			Local<String> local;
			if (maybe.ToLocal(&local)) {
				_query_string = FromV8String(local);
			}
			_timeout = getint32(query_object, "query_timeout");	
			_polling = getbool(query_object, "query_polling"); 
			_query_tz_adjustment = getint32(query_object, "query_tz_adjustment");
			_id = getint32(query_id); // getint64(query_id); //  query_id->NumberValue();
		}

	private:
		int32_t getint32(Local<Object> query_object, const char *v)
		{
			nodeTypeFactory fact;
			auto context = fact.isolate->GetCurrentContext();
			auto l = get(query_object, v);
			if (!l->IsNull())
			{
				auto maybe = l->ToInt32(context);
				Local<Int32> local;
				if (maybe.ToLocal(&local))
				{
					return local->Value();
				}
			}
			return 0;
		}

		int64_t getint64(Local<Object> query_object, const char *v)
		{
			nodeTypeFactory fact;
			auto context = fact.isolate->GetCurrentContext();
			auto l = get(query_object, v);
			if (!l->IsNull())
			{
				auto maybe = l->ToBigInt(context);
				Local<BigInt> local;
				if (maybe.ToLocal(&local))
				{
					return local->Int64Value();
				}
			}
			return 0;
		}

		int64_t getint64(Handle<Number> l)
		{
			nodeTypeFactory fact;
			auto context = fact.isolate->GetCurrentContext();
			if (!l->IsNull())
			{
				auto maybe = l->ToBigInt(context);
				Local<BigInt> local;
				if (maybe.ToLocal(&local))
				{
					return local->Int64Value();
				}
			}
			return 0;
		}

		int32_t getint32(Handle<Number> l)
		{
			nodeTypeFactory fact;
			auto context = fact.isolate->GetCurrentContext();
			if (!l->IsNull())
			{
				auto maybe = l->ToInt32(context);
				Local<Int32> local;
				if (maybe.ToLocal(&local))
				{
					return local->Value();
				}
			}
			return 0;
		}

		bool getbool(Local<Object> query_object, const char *v)
		{
			nodeTypeFactory fact;
			auto context = fact.isolate->GetCurrentContext();
			auto l = get(query_object, v);
			if (!l->IsNull())
			{
				auto maybe = l->ToBoolean(context);
				Local<Boolean> local;
				if (maybe.ToLocal(&local))
				{
					return local->Value();
				}
			}
			return 0;
		}

		Local<Value> get(Local<Object> o, const char *v)
		{
			nodeTypeFactory fact;
			auto context = fact.isolate->GetCurrentContext();
			const auto vp = fact.new_string(v);
			auto maybe = o->Get(context, vp);
			Local<Value> local;
			if (maybe.ToLocal(&local)) {
				return local;
			}
			return fact.null();
		}

		wstring _query_string;
		int32_t _timeout;
		int32_t _query_tz_adjustment;
		int64_t _id;
		bool _polling;
	};
}

