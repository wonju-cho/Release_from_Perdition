#pragma once

#include "CoreMinimal.h"

class FUIManager
{
	public:
	
	static FUIManager& Get()
	{
		static FUIManager Instance;
		return Instance;
	}

	float GetDPIScale();

	private:
	FUIManager() {}
	FUIManager(const FUIManager&) = delete;
	FUIManager& operator=(const FUIManager&) = delete;

	void Initialize();

	private:
	bool bIsInitialized = false;
	float DPIScale = 1.0f;
	FMargin SafeZonePadding;
};
