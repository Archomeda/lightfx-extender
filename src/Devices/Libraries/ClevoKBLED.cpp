
#include <Windows.h>  
#include <iostream>  
#include <string> 
#include <WbemCli.h>  
#include <atlbase.h>
#include <comutil.h>
#include <comdef.h>
#include <thread>

#include "ClevoKBLED.h"

#include "../../Timelines/LightColor.h"
#include "../../Utils/Log.h"

#pragma comment(lib, "wbemuuid.lib")  

using std::cout;
using std::cin;
using std::endl;
using std::hex;
using std::vector;
using std::string;

using namespace lightfx::timelines;
using namespace lightfx::utils;

// Functions
// Initialize connection to Clevo WMI Class
HRESULT ClevoKBLED::Initialize(){

	HRESULT hRes;

	// Initialize COM. ------------------------------------------
	if (FAILED(hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		LOG_WARNING(L"Unable to launch COM: 0x" + std::to_wstring(hRes));
		cout << "Unable to launch COM: 0x" << hex << hRes << endl;
		//return hRes;
	}

	// Set general COM security levels --------------------------
	if ((FAILED(hRes = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_CONNECT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0))))
	{
		LOG_WARNING(L"Unable to initialize security: 0x"+ std::to_wstring(hRes));
		cout << "Unable to initialize security: 0x" << hex << hRes << endl;
		//return hRes;
	}

	// Obtain the initial locator to WMI -------------------------
	IWbemLocator* pLocator = NULL;
	if (FAILED(hRes = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pLocator))))
	{
		LOG_WARNING(L"Unable to create a WbemLocator: 0x" + std::to_wstring(hRes));
		cout << "Unable to create a WbemLocator: 0x" << hex << hRes << endl;
		return hRes; // Program has failed. 
	}

	// Connect to WMI through the IWbemLocator::ConnectServer method
	// IWbemServices* pService = NULL;
	if (FAILED(hRes = pLocator->ConnectServer(_bstr_t(L"root\\WMI"), NULL, NULL, NULL, WBEM_FLAG_CONNECT_USE_MAX_WAIT, NULL, NULL, &pService)))
	{
		pLocator->Release();
		LOG_WARNING(L"Unable to connect to \"root\\WMI\": 0x" + std::to_wstring(hRes));
		cout << "Unable to connect to \"root\\WMI\": 0x" << hex << hRes << endl;
		return hRes; // Program has failed. 
	}


	// Set security levels on the proxy ------------------------- 
	if (FAILED(hRes = CoSetProxyBlanket(pService, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE)))
	{
		LOG_WARNING(L"Could not set proxy blanket. Error code = 0x");
		cout << "Could not set proxy blanket. Error code = 0x" << hex << hRes << endl;
		pService->Release();
		pLocator->Release();
		CoUninitialize();
		return hRes; // Program has failed. 
	}

	// Query the COM system. ------------------------------------
	IEnumWbemClassObject* pEnumerator = NULL;
	if (FAILED(hRes = pService->ExecQuery(_bstr_t(L"WQL"), _bstr_t(L"SELECT * FROM CLEVO_GET"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator)))
	{
		LOG_WARNING(L"Query for CLEVO_GET failed. Error code = 0x");
		cout << "Query for CLEVO_GET failed. Error code = 0x" << hex << hRes << endl;
		pService->Release();
		pLocator->Release();
		CoUninitialize();
		return hRes; // Program has failed.
	}

	// Iterate trough the results. ------------------------------------
	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		hRes = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		// Get SetKBLED Parameters
		IWbemClassObject *clevo = NULL;
		IWbemClassObject *params = NULL;
		// IWbemClassObject *paramsInst = NULL;

		pService->GetObject(_bstr_t(L"CLEVO_GET"), 0, NULL, &clevo, NULL);

		if (FAILED(hRes = clevo->GetMethod(_bstr_t(L"SetKBLED"), 0, &params, NULL))) {
			LOG_WARNING(L"could not get method SetKBLED. Error code = 0x");
			cout << "could not get method SetKBLED. Error code = 0x" << hex << hRes << endl;
			return hRes; // Program has failed. 
		}

		if (FAILED(hRes = params->SpawnInstance(0, &paramsInst))) {
			LOG_WARNING(L"could not spawn params instance. Error code = 0x" );
			cout << "could not spawn params instance. Error code = 0x" << hex << hRes << endl;
			return hRes; // Program has failed. 
		}

		// This Pointer
		CIMTYPE type;
		LONG flavor;
		pclsObj->Get(L"__PATH", 0, &this_pointer, &type, &flavor);

		// Cleanup
		pclsObj->Release();
		pEnumerator->Release();
		pLocator->Release();

		return 0;
	}

	return 1;
}

// Release Resources
HRESULT ClevoKBLED::Release() {
	// TODO: Restore keyboard lights from before

	if (pService != NULL) {
		pService->Release();
	}

	if (paramsInst != NULL) {
		paramsInst->Release();
	}

	// TODO: Check if everything is being cleaned up
	CoUninitialize();
	return 0;
}

// Set KBLED: Pass RAW hex value
HRESULT ClevoKBLED::SetKBLED(uint32_t hex_value) {

	// LOG_WARNING(L"Updating KB Lights (CLEVO)");

	HRESULT hRes;

	// Set SetKBLED Parameters
	CComVariant paramVt;
	paramVt.vt = VT_INT;
	paramVt.intVal = hex_value; // VALUES: 0x00001000 -> OFF  // 0x00001000 -> ON
	const HRESULT n = paramVt.ChangeType(VT_I4);

	if (FAILED(hRes = paramsInst->Put(_bstr_t(L"Data"), 0, &paramVt, NULL))) {
		LOG_WARNING(L"CLEVO: could not set the argument. Error code = 0x" + std::to_wstring(hRes));
		cout << "could not set the argument. Error code = 0x" << hex << hRes << endl;
	}

	// Execute
	IWbemClassObject *results = NULL;
	hRes = pService->ExecMethod(this_pointer.bstrVal, _bstr_t(L"SetKBLED"), 0, NULL, paramsInst, &results, NULL);
	LOG_DEBUG(L"CLEVO: EXECUTE. Error code = 0x" + std::to_wstring(hRes));

	// Clear Data
	VariantClear(&paramVt);

	return 0;
}

// Set KBLED: Using RGB
HRESULT ClevoKBLED::SetKBLED(KBLEDAREA area, uint8_t color_r, uint8_t color_g, uint8_t color_b) {
	// cout << hex << (((((area << 8) + color_r) << 8) + color_g) << 8) + color_b;
	return ClevoKBLED::SetKBLED((((((area << 8) + color_r) << 8) + color_g) << 8) + color_b);
}

// Set KBLED: Using RGBA
HRESULT ClevoKBLED::SetKBLED(KBLEDAREA area, uint8_t color_r, uint8_t color_g, uint8_t color_b, double alpha) {
	if (alpha >= 1) { // Ignore all values over 1
		return ClevoKBLED::SetKBLED(area, color_r, color_g, color_b);
	} else {
		return ClevoKBLED::SetKBLED(area, ClevoKBLED::ToInt(color_r*alpha), ClevoKBLED::ToInt(color_g*alpha), ClevoKBLED::ToInt(color_b*alpha));
	}
}

// Set KBLED Mode
HRESULT ClevoKBLED::SetKBLEDMode(KBLEDMODE mode) {
	return ClevoKBLED::SetKBLED(mode);
}

// Helper Functions
int ClevoKBLED::ToInt(double x) {
	double dx = x < 0.0 ? -0.5 : 0.5;
	return static_cast<int>(x + dx);
}

template< typename T >
string ClevoKBLED::intToHexStr(T i){
	std::stringstream stream;
	stream << "0x"
		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;
	return stream.str();
}
