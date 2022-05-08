#pragma once
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include "Vector.h"

namespace UnitTestLibraryDesktop
{
	class JsonIntegerParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonIntegerParseHelper, FieaGameEngine::IJsonParseHelper);

	public:
		class Wrapper final : public FieaGameEngine::JsonParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(Wrapper, FieaGameEngine::JsonParseCoordinator::Wrapper);

		public:
			virtual void Initialize() override;
			virtual gsl::owner<Wrapper*> Create() const override;

			FieaGameEngine::Vector<std::int32_t> _data;
		};

		virtual gsl::owner<IJsonParseHelper*> Create() const override;
		virtual void Initialize() override;
		virtual bool StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value, size_t arrIndex) override;
		virtual bool EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value) override;

	private:
		inline static const std::string IntegerKey = "integer";
		bool _parsingData = false;
	};
}