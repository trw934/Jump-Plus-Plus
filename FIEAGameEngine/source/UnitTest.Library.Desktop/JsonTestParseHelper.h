#pragma once
#include <cstdint>
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"

namespace UnitTestLibraryDesktop
{
	class JsonTestParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTestParseHelper, FieaGameEngine::IJsonParseHelper)

	public:
		class Wrapper final : public FieaGameEngine::JsonParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(Wrapper, FieaGameEngine::JsonParseCoordinator::Wrapper)

		public:
			virtual void Initialize() override;
			virtual gsl::owner<Wrapper*> Create() const override;

			std::uint32_t MaxDepth = 0;
		};

		virtual gsl::owner<IJsonParseHelper*> Create() const override;
		virtual void Initialize() override;
		virtual bool StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value, size_t arrIndex) override;
		virtual bool EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value) override;

		bool InitializeCalled = false;
		std::uint32_t StartHandlerCount = 0;
		std::uint32_t EndHandlerCount = 0;
		std::uint32_t ArrayElementCount = 0;
	};
}