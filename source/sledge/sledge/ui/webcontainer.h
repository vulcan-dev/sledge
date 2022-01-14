#pragma once

#pragma warning (push)
#pragma warning (disable: 4201)
#pragma warning (disable: 4100)
#include <Ultralight/Ultralight.h>
#pragma warning (pop)

#include <mutex>
#include <future>
#include <vector>

class CWebContainer {
public:
	CWebContainer(unsigned int iWidth, unsigned int iHeight, int iX = 0, int iY = 0);
	~CWebContainer();

	void LoadURL(const char* cURL);

	void Resize(unsigned int iWidth, unsigned int iHeight);
	void Reload();

	ultralight::RefPtr<ultralight::View> View() { return m_View; }

	void Draw();

	int m_X, m_Y;

	unsigned int m_Id;
private:
	char* cLastURL;
	unsigned int m_Width, m_Height;
	ultralight::RefPtr<ultralight::View> m_View;

};

namespace WebContainers {
	inline std::mutex RegisteredMutex;
	inline std::map<unsigned int, CWebContainer*> RegisteredContainers;

	inline std::mutex FutureMutex;
	inline std::vector<std::shared_future<void>*> FutureQueue;

	inline unsigned int iLastId = 0;

	inline bool bResetting = false;

	inline void Draw() {
		RegisteredMutex.lock();
		for (auto Pair = WebContainers::RegisteredContainers.begin(); Pair != WebContainers::RegisteredContainers.end(); Pair++) { Pair->second->Draw(); }
		RegisteredMutex.unlock();
	}

	inline void RegisterContainer(CWebContainer* Container) {
		if (bResetting) return;
		RegisteredMutex.lock();
		RegisteredContainers[iLastId] = Container;
		Container->m_Id = iLastId;
		RegisteredMutex.unlock();
	}

	inline void UnregisterContainer(CWebContainer* Container) {
		if (bResetting) return;
		RegisteredMutex.lock();
		RegisteredContainers.erase(Container->m_Id);
		RegisteredMutex.unlock();
	}

	inline void AddFuture(std::shared_future<void>* pFuture) {
		FutureMutex.lock();
		FutureQueue.push_back(pFuture);
		FutureMutex.unlock();
	}

	inline void InvokeFutures() {
		FutureMutex.lock();
		for (std::shared_future<void>* Future : FutureQueue) { Future->wait(); }
		FutureQueue.clear();
		FutureMutex.unlock();
	}

	inline void WaitForFutures() {
		while (true) {
			if (WebContainers::FutureQueue.size() == 0) break;
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}
	}

	inline void BeginReset() {
		bResetting = true;
		RegisteredMutex.lock();
		FutureQueue.clear();
		while (bResetting) { std::this_thread::sleep_for(std::chrono::milliseconds(50)); }
		RegisteredMutex.unlock();
	}

	inline void EndReset() {
		bResetting = false;
	}

}