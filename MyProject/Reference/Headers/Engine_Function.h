#pragma once


namespace Engine
{
	template <typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete [] pPointer;
			pPointer = nullptr;
		}
	}

	template <typename T>
	unsigned long Safe_AddRef(T& pInstance)
	{
		unsigned long	dwRefCnt = 0;
		if (nullptr != pInstance)		
			dwRefCnt = pInstance->AddRef();			
		
		return dwRefCnt;
	}

	template <typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long	dwRefCnt = 0;
		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();
			if (0 == dwRefCnt)
				pInstance = nullptr;			
		}
		return dwRefCnt;
	}

	class CTagFinder
	{
	public:
		explicit CTagFinder(const wchar_t* pTag) : m_pTag(pTag) {}
		virtual ~CTagFinder() = default;
	public:
		template <typename T>
		bool operator () (T& Pair)
		{
			return !lstrcmp(Pair.first, m_pTag);
		}
	private:
		const wchar_t*		m_pTag = nullptr;
	};

	class CTagFinderWstring
	{
	public:
		explicit CTagFinderWstring(const wchar_t* _pTag) :m_pTag(_pTag) {};
		virtual ~CTagFinderWstring() = default;

	public:
		template <typename T>
		bool operator()(const T& _Pair)
		{
			return !lstrcmp(_Pair.first.c_str(), m_pTag);
		}
	private:
		const wchar_t* m_pTag = nullptr;
	};

	static  _float2 Get_Mouse(HWND _hWnd)
	{
		POINT pt{};

		GetCursorPos(&pt);
		ScreenToClient(_hWnd, &pt);

		return _float2((float)pt.x, (float)pt.y);
	}
}