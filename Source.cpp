#include <iostream>
#include <windows.h>
#include <netlistmgr.h>
#include "ConnectivityMonitor.h"

int failCheck(HRESULT hr) {
	if (FAILED(hr)) {
		std::cout << "Failed to get container" << std::endl;
		return -1;
	}
	else if (SUCCEEDED(hr)) {
		std::cout << "All good" << std::endl;
		return -1;
	}
}

void printConnectivityDetails(INetworkListManager* pNWLM) {
	IEnumNetworkConnections* iENC = NULL;
	HRESULT networkCheck = pNWLM->GetNetworkConnections((IEnumNetworkConnections**)&iENC); // the stupidest line of code i have ever written love
	failCheck(networkCheck);
	INetworkConnection* in[10];
	ULONG elementNum = NULL;
	iENC->Next(10, in, &elementNum);
	std::cout << in[0] << std::endl; // Check out functions on in[0] here
}

int main() {
	std::cout << "Running" << std::endl;
	INetworkListManager* pNWLM;
	INetworkListManagerEvents* pNWLME;
	std::string debounce;

	CoInitialize(NULL); // We have to do this first to tell the computer hey im gonna use COM!

	HRESULT comState = CoCreateInstance(CLSID_NetworkListManager, NULL, CLSCTX_ALL, IID_INetworkListManager, (void**)&pNWLM);

	// The reason why we use succeeded here over any just == ois because S_OK can return so much and specifically SUCCEEDED checks for a confirmed successive code
	if(SUCCEEDED(comState)) {
		std::cout << "Success" << std::endl;

		NLM_CONNECTIVITY newConn;
		// Remember that HResult in this context still emans just handling error and success codes
		ConnectivityMonitor* conMon = new ConnectivityMonitor();
		IConnectionPointContainer* pContainer = NULL;
		IConnectionPoint* pConnectionPoint = NULL;
		HRESULT hr = pNWLM->QueryInterface(IID_IConnectionPointContainer, (void**)&pContainer);
		failCheck(hr);
		HRESULT hr2 = pContainer->FindConnectionPoint(IID_INetworkListManagerEvents, &pConnectionPoint);
		failCheck(hr2);
		DWORD cookie; // for subscribing
		HRESULT hr3 = pConnectionPoint->Advise(conMon, &cookie);
		failCheck(hr3);
		std::cin.get();

		std::cout << "Monitoring... Press Ctrl+C to exit" << std::endl;
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}



	} else if (FAILED(comState)) {
		std::cout << "FAILED" << std::endl;
		std::cout << std::hex << comState << std::endl;
	}	


} 

