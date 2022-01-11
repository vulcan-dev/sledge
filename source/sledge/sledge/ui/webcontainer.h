#pragma once

#pragma warning (push)
#pragma warning (disable: 4201)
#pragma warning (disable: 4100)
#include <Ultralight/Ultralight.h>
#pragma warning (pop)

#include <vector>
#include <mutex>
#include <future>

/*
	to-do:
		the thread safety implementation here could *definitely* be improved, by a ton, this is probably really heavy on performance
*/

class CWebContainer {
public:
	CWebContainer(unsigned int iWidth, unsigned int iHeight, int iX = 0, int iY = 0);

	~CWebContainer();

	void LoadURL(const char* cURL);
	void Draw();

	void Resize(int iWidth, int iHeight);

	int m_X, m_Y;

	ultralight::RefPtr<ultralight::View> View() { return m_View; }
private:
	unsigned int m_Width, m_Height;
	ultralight::RefPtr<ultralight::View> m_View;
};

namespace WebContainers {
	inline std::mutex RegisterMutex;
	inline std::vector<CWebContainer*> RegisteredContainers;

	inline std::mutex FutureMutex;
	inline std::vector<std::shared_future<void>*> FutureQueue;

	inline void AddFuture(std::shared_future<void>* pFuture) {
		FutureMutex.lock();
		FutureQueue.push_back(pFuture);
		FutureMutex.unlock();
	}

	inline void CallFutures() {
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
}