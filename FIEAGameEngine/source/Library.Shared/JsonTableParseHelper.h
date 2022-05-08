#pragma once
#include <cstdint>
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include "Scope.h"
#include "Stack.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Struct that represents a frame of context for scope parsing
	/// </summary>
	struct ContextFrame
	{
		/// <summary>
		/// Constructs a context frame for implementing pushdown automata
		/// </summary>
		/// <param name="key">Key for key for context frame</param>
		/// <param name="scope">Scope for context frame</param>
		ContextFrame(const std::string& key, const std::string& className, Scope& scope) :
			_key{ key }, _className{ className }, _scope{ &scope }
		{
		}

		const std::string& _key;
		std::string _className;
		Scope* _scope{ nullptr };
	};

	class JsonTableParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper);

	public:
		/// <summary>
		/// Class that wraps a scope for parsing purposes
		/// </summary>
		class Wrapper final : public JsonParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(Wrapper, JsonParseCoordinator::Wrapper);

		public:
			/// <summary>
			/// Constructs a wrapper using the given scope
			/// </summary>
			/// <param name="scope">Scope used by the wrapper</param>
			Wrapper(Scope& scope);

			/// <summary>
			/// Initializes the wrapper
			/// </summary>
			virtual void Initialize() override;

			/// <summary>
			/// Creates an instance of the wrapper
			/// </summary>
			/// <returns>Pointer to the created wrapper instance</returns>
			virtual gsl::owner<Wrapper*> Create() const override;

			Scope* _scope{ nullptr };
		};

		/// <summary>
		/// Creates an instance of this helper
		/// </summary>
		/// <returns>Pointer to the created wrapper instance</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const override;

		/// <summary>
		/// Initializes the typeMap and context for the handler
		/// </summary>
		virtual void Initialize() override;

		/// <summary>
		/// Clears the typeMap and context for the handler
		/// </summary>
		virtual void Cleanup() override;

		/// <summary>
		/// Attempts to handle the key/value pair and parses it into the wrapper
		/// </summary>
		/// <param name="wrapper">The wrapper to use for the data.</param>
		/// <param name="key">The key to use for parsing</param>
		/// <param name="value">The value to use for parsing</param>
		/// <returns>True if the handler can handle the key/value pair, false if not</returns>
		virtual bool StartHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value, size_t arrIndex) override;

		/// <summary>
		/// Attempts to complete the handling of the key/value pair
		/// </summary>
		/// <param name="wrapper">The wrapper to use for the data</param>
		/// <param name="key">The key to use for parsing</param>
		/// <param name="value">The value to use for parsing</param>
		/// <returns>True if the handler handled the key/value pair, false if not</returns>
		virtual bool EndHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value) override;

	private:
		inline static const std::string TypeKey = "type";
		inline static const std::string ValueKey = "value";
		inline static const std::string ClassKey = "class";
		Hashmap<std::string, DatumTypes> _typeMap;
		Stack<ContextFrame> _context;
	};
}