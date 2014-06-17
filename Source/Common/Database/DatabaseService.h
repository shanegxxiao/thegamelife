#ifndef __DatabaseService_h__
#define __DatabaseService_h__

#include "Runtime/Object.h"
#include "Connection.h"

namespace Database
{
	class DatabaseService : public Runtime::Object
	{
	private:
		typedef std::vector<Connection*> ConnectionList;

	public:
		DatabaseService();
		virtual ~DatabaseService();

		virtual bool Initialize(Runtime::ParamList& kParamList);
		virtual void Running();
		virtual bool Shutdown();
		virtual Runtime::Object::ObjectType GetType();

		Connection* Connect(const std::string& strConStr);
		bool CloseConnection(Connection* pkConnection);

	private:
		ConnectionList mkConnectionList;
	};

	inline Runtime::Object::ObjectType DatabaseService::GetType()
	{
		return Runtime::Object::OT_SERVICE;
	}
}

#endif//__DatabaseMgr_h__