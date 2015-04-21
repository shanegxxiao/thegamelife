#ifndef __OtlDirectExecuteTask_h__
#define __OtlDirectExecuteTask_h__

#include "OtlTask.h"

namespace Database
{
	class OtlDirectExecuteTask : public OtlTask
	{
	public:
		OtlDirectExecuteTask();
		virtual ~OtlDirectExecuteTask();

		void setExecuteSQL(const std::string& strSQL);
		std::string& getExecuteSQL();

	protected:
		virtual void onExecute(OtlConnection* pkConnection);
		virtual void onFinish();

	private:
		unsigned long mulAffectRows;
		std::string mStrSQL;
	};

	inline void OtlDirectExecuteTask::setExecuteSQL(const std::string& strSQL)
	{
		mStrSQL = strSQL;
	}

	inline std::string& OtlDirectExecuteTask::getExecuteSQL()
	{
		return mStrSQL;
	}
}


#endif//__DirectExecuteTask_h__