#ifndef __DirectExecuteTask_h__
#define __DirectExecuteTask_h__

#include "Task.h"

namespace Database
{
	class DirectExecuteTask : public Task
	{
	public:
		DirectExecuteTask();
		virtual ~DirectExecuteTask();

		void setExecuteSQL(const std::string& strSQL);
		std::string& getExecuteSQL();

	protected:
		virtual void onExecute(Connection* pkConnection);
		virtual void onFinish();

	private:
		unsigned long mulAffectRows;
		std::string mStrSQL;
	};

	inline void DirectExecuteTask::setExecuteSQL(const std::string& strSQL)
	{
		mStrSQL = strSQL;
	}

	inline std::string& DirectExecuteTask::getExecuteSQL()
	{
		return mStrSQL;
	}
}


#endif//__DirectExecuteTask_h__