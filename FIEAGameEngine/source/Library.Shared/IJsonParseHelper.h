#pragma once
#include <cstdint>
#include <string>
#include <cstddef>
#include <stdexcept>
#include <cassert>
#include "RTTI.h"
#include "Factory.h"
#include "glm\glm.hpp"
#include "gsl/gsl"
#pragma warning (push)
#pragma warning (disable : 26812)
#include "json/json.h"
#pragma warning (pop)
#include "JsonParseCoordinator.h"

namespace FieaGameEngine
{
	class IJsonParseHelper : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI);
	public:
		/// <summary>
		/// Initializes the helper
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Cleans up the helper
		/// </summary>
		virtual void Cleanup();

		/// <summary>
		/// Attempts to handle the key/value pair and parses it into the wrapper
		/// </summary>
		/// <param name="wrapper">The wrapper to use for the data.</param>
		/// <param name="key">The key to use for parsing</param>
		/// <param name="value">The value to use for parsing</param>
		/// <returns>True if the handler can handle the key/value pair, false if not</returns>
		virtual bool StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value, size_t arrIndex) = 0;

		/// <summary>
		/// Attempts to complete the handling of the key/value pair
		/// </summary>
		/// <param name="wrapper">The wrapper to use for the data</param>
		/// <param name="key">The key to use for parsing</param>
		/// <param name="value">The value to use for parsing</param>
		/// <returns>True if the handler handled the key/value pair, false if not</returns>
		virtual bool EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value) = 0;

		/// <summary>
		/// Creates an instance of the helper
		/// </summary>
		/// <returns>Pointer to the created helper</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const = 0;

		/// <summary>
		/// Destructs a helper
		/// </summary>
		virtual ~IJsonParseHelper() = default;
	};
}