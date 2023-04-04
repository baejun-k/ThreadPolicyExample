#ifndef __THREAD_POLICY_H__
#define __THREAD_POLICY_H__


#include <mutex>
#include <assert.h>
#include <type_traits>


class SingleThreadPolicy
{
protected:
	SingleThreadPolicy() {}
	~SingleThreadPolicy() {}

public:
	class Lock
	{
	public:
		Lock() = delete;
		Lock(SingleThreadPolicy&&) = delete;
		explicit Lock(SingleThreadPolicy& obj) {}
		~Lock() {}

		Lock& operator=(Lock&&) = delete;
		Lock& operator=(const Lock&) = delete;
	};
};


struct StdMultiThreadPolicy
{
private:
	class Token
	{
	public:
		bool m_isInit;
		std::recursive_mutex m_mutex;

		Token()
			: m_isInit(false)
			, m_mutex()
		{
			m_isInit = true;
		}
		~Token()
		{
			assert(m_isInit);
		}
	};

	Token m_token;

protected:
	StdMultiThreadPolicy()
		: m_token()
	{}
	~StdMultiThreadPolicy()
	{}

public:
	class Lock;
	friend class Lock;

	class Lock
	{
	private:
		StdMultiThreadPolicy::Token& m_token;

	public:
		Lock() = delete;
		Lock(StdMultiThreadPolicy&&) = delete;
		explicit Lock(StdMultiThreadPolicy& policy)
			: m_token(policy.m_token)
		{
			assert(m_token.m_isInit);
			m_token.m_mutex.lock();
		}
		~Lock()
		{
			assert(m_token.m_isInit);
			m_token.m_mutex.unlock();
		}

		Lock& operator=(StdMultiThreadPolicy&&) = delete;
		Lock& operator=(const StdMultiThreadPolicy&) = delete;
	};
};


#endif // !__THREAD_POLICY_H__