#include <dxgi1_4.h>
#include <iostream>

int main(int argc, char** argv)
{
	// Initializies COM for older win versions
	HRESULT init_result = CoInitializeEx(nullptr, 0);

	if (!SUCCEEDED(init_result))
	{
		std::cerr << "Unable to initialize IDXGIFactory " << init_result << std::endl;
		return -1;
	}

	IDXGIFactory* factory = nullptr;
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));

	if (!SUCCEEDED(result))
	{
		std::cerr << "Unable to create IDXGIFactory " << result << std::endl;
		return -1;
	}

	UINT adapterIndex = 0;
	IDXGIAdapter* adapter = nullptr;
	IDXGIAdapter* bestAdapter = nullptr;
	DXGI_ADAPTER_DESC adapterDesc;
	size_t bestAdapterVideoMemory = 0;

	while (true)
	{
		result = factory->EnumAdapters(adapterIndex, &adapter);

		if (SUCCEEDED(result))
		{
			adapterIndex++;

			result = adapter->GetDesc(&adapterDesc);

			if (!SUCCEEDED(result))
			{
				std::cerr << "Unable to get IDXGIAdapter description " << result << std::endl;
				return -1;
			}

			std::wcout << adapterDesc.Description << " "
				<< adapterDesc.DedicatedVideoMemory / (1024 * 1024) << " "
				<< adapterDesc.DedicatedSystemMemory / (1024 * 1024) << " "
				<< adapterDesc.SharedSystemMemory / (1024 * 1024) << std::endl;

			if (bestAdapter == nullptr || adapterDesc.DedicatedVideoMemory > bestAdapterVideoMemory)
			{
				bestAdapter = adapter;
				bestAdapterVideoMemory = adapterDesc.DedicatedVideoMemory;
			}
		}
		else if (result == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}
		else
		{
			std::cerr << "Unable to enumerate IDXGIAdapter" << result << std::endl;
		}
		std::cout << bestAdapter << std::endl;
	}

	IDXGIOutput* output = nullptr;
	UINT outputIndex = 0;
	DXGI_OUTPUT_DESC outputDescription;

	while (true)
	{
		result = bestAdapter->EnumOutputs(outputIndex, &output);

		if (SUCCEEDED(result))
		{
			outputIndex++;

			result = output->GetDesc(&outputDescription);

			if (!SUCCEEDED(result))
			{
				std::cerr << "Unable to get IDXGIOutput description" << result << std::endl;
				return -1;
			}

			std::wcout << outputDescription.DeviceName << " "
				<< outputDescription.DesktopCoordinates.left << " "
				<< outputDescription.DesktopCoordinates.top << " "
				<< outputDescription.DesktopCoordinates.right << " "
				<< outputDescription.DesktopCoordinates.bottom << " "
				<< std::endl;
		}
		else if (result == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}
		else
		{
			std::cerr << "Unable to enumerate IDXGIOutput" << result << std::endl;
		}
		std::cout << bestAdapter << std::endl;
	}
	return 0;
}