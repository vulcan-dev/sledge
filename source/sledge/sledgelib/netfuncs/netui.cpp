#include "sledge/ui/webcontainer.h"

#include "util/math.h"

#include <future>

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func unsigned int ContainerCreate(unsigned int iWidth, unsigned int iHeight, int iX, int iY) {
	unsigned int iHandle = WebContainers::iLastId;

	std::shared_future Future = std::async(std::launch::deferred, [](int iWidth, int iHeight, int iX, int iY) { new CWebContainer(iWidth, iHeight, iX, iY); }, iWidth, iHeight, iX, iY);
	WebContainers::AddFuture(&Future);
	WebContainers::WaitForFutures();

	return iHandle;
}

sledgelib_func void ContainerDestroy(unsigned int iHandle) {
	CWebContainer* Container = WebContainers::RegisteredContainers[iHandle];
	if (!Container) return;

	std::shared_future Future = std::async(std::launch::deferred, [](CWebContainer* Container) { delete Container; }, Container);
	WebContainers::AddFuture(&Future);
	WebContainers::WaitForFutures();
}

sledgelib_func void ContainerLoadURL(unsigned int iHandle, char* cURL) {
	CWebContainer* Container = WebContainers::RegisteredContainers[iHandle];
	if (!Container) return;
	
	std::shared_future Future = std::async(std::launch::deferred, [](CWebContainer* Container, char* cURL) { Container->LoadURL(cURL); }, Container, cURL);
	WebContainers::AddFuture(&Future);
	WebContainers::WaitForFutures();
}

sledgelib_func void ContainerSetPos(unsigned int iHandle, Vector2 vPosition) {
	CWebContainer* Container = WebContainers::RegisteredContainers[iHandle];
	if (!Container) return;
	Container->m_X = static_cast<int>(vPosition.x); Container->m_Y = static_cast<int>(vPosition.y);
}

sledgelib_func void ContainerSetSize(unsigned int iHandle, Vector2 vSize) {
	CWebContainer* Container = WebContainers::RegisteredContainers[iHandle];
	if (!Container) return;
	Container->Resize(static_cast<unsigned int>(vSize.x), static_cast<unsigned int>(vSize.y));
}

sledgelib_func void ContainerRunJS(unsigned int iHandle, char* cJS) {
	CWebContainer* Container = WebContainers::RegisteredContainers[iHandle];
	if (!Container) return;
	Container->View()->EvaluateScript(cJS);
}