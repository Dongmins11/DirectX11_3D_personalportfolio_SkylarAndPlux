#pragma once

#define GET_INSTANCE(CLASSNAME)	[](){											\
	CLASSNAME*	pInstance = CLASSNAME::Get_Instance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->AddRef();}														\
	return pInstance;															\
	}();

#define RELEASE_INSTANCE(CLASSNAME)	[](){										\
	CLASSNAME*	pInstance = CLASSNAME::Get_Instance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->Release();}														\
}();


#define MSGBOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Message"), MB_OK)

#define BEGIN(NAMESPACE) namespace NAMESPACE {

#define END }

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif


#define DECLARE_SINGLETON(ClassName)							\
public:															\
static ClassName* Get_Instance()								\
{																\
	if (!m_pInstance)											\
		m_pInstance = new ClassName;							\
	return m_pInstance;											\
}																\
static unsigned long Destroy_Instance()							\
{																\
	unsigned long		dwRefCnt = 0;							\
	if (nullptr != m_pInstance)									\
	{															\
		dwRefCnt = m_pInstance->Release();						\
		if(0 == dwRefCnt)										\
			m_pInstance = nullptr;								\
	}															\
	return dwRefCnt;											\
}																\
private:														\
	static ClassName* m_pInstance;								

#define IMPLEMENT_SINGLETON(ClassName)							\
ClassName* ClassName::m_pInstance = nullptr; 

