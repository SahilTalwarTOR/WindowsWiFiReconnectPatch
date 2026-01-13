#pragma once
#include <netlistmgr.h>
#include <iostream>

class ConnectivityMonitor : public INetworkListManagerEvents {
private:
	LONG refCount;
public:

	ConnectivityMonitor() {
		refCount = 1; // For every instance of this thats made, we essentially set the ref to start at 1
	}

	HRESULT ConnectivityChanged(NLM_CONNECTIVITY newConn) {
		if (newConn == NLM_CONNECTIVITY_DISCONNECTED) {
			std::cout << "Network disconnect" << std::endl;
		}
		else if (newConn == NLM_CONNECTIVITY_IPV4_INTERNET || NLM_CONNECTIVITY_IPV6_INTERNET) {
				std::cout << "Connecting, or connected" << std::endl;
		}
		return S_OK;
	}

	HRESULT QueryInterface(REFIID riid, void** ppvObject) {
		if (riid == IID_INetworkListManagerEvents || riid == IID_IUnknown) {
			*ppvObject = this; // We basically say hey, this is us btw
			AddRef(); // ?
			return S_OK;
		}
		else {
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
	}
	ULONG AddRef() {
		this->refCount = this->refCount + 1;
		return refCount;
	}

	ULONG Release() {
		this->refCount = this->refCount - 1;
		if (this->refCount <= 0) {
			delete this; // delete if not being used ofc
			return 0;
		}
		return refCount; // otherwise, we return refcount
	}


};