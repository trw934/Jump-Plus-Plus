#pragma once
#include <cstdint>
#include <string>
#include <cstddef>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <fstream>
#include "RTTI.h"
#include "glm\glm.hpp"
#include "gsl/gsl"
#pragma warning (push)
#pragma warning (disable : 26812)
#include "json/json.h"
#pragma warning (pop)
#include "Vector.h"

namespace FieaGameEngine
{
	class IJsonParseHelper;
	class JsonParseCoordinator final
	{
	public:
		class Wrapper : public RTTI
		{
			friend class JsonParseCoordinator;
			RTTI_DECLARATIONS(Wrapper, RTTI);

		public:
			/// <summary>
			/// Initializes the wrapper
			/// </summary>
			virtual void Initialize();

			/// <summary>
			/// Creates an instance of the wrapper
			/// </summary>
			/// <returns>Pointer to the created wrapper instance</returns>
			virtual gsl::owner<Wrapper*> Create() const = 0;

			/// <summary>
			/// Gets the JsonParseCoordinator associated with this wrapper
			/// </summary>
			/// <returns>Pointer to the JsonParseCoordinator</returns>
			JsonParseCoordinator* GetJsonParseCoordinator();

			/// <summary>
			/// Gets the JsonParseCoordinator associated with this wrapper
			/// </summary>
			/// <returns>Pointer to the JsonParseCoordinator</returns>
			const JsonParseCoordinator* GetJsonParseCoordinator() const;

			/// <summary>
			/// Increments the depth counter of the wrapper
			/// </summary>
			void IncrementDepth();

			/// <summary>
			/// Decrements the depth counter of the wrapper
			/// </summary>
			void DecrementDepth();

			/// <summary>
			/// Returns the current depth of the wrapper
			/// </summary>
			/// <returns>Current depth of the wrapper</returns>
			std::uint32_t Depth() const;
		private:
			void SetJsonParseCoordinator(JsonParseCoordinator& parseCoordinator);
			
			JsonParseCoordinator* _parseCoordinator{ nullptr };
			std::uint32_t _depth{ 0 };
		};
		/// <summary>
		/// Initializes the parse coordinator with the given wrapper
		/// </summary>
		/// <param name="wrapper">Wrapper to be used in construction</param>
		JsonParseCoordinator(Wrapper& wrapper);

		/// <summary>
		/// Destroys the parse coordinator. If the coordinator is a clone, it deletes all helpers and the wrapper associated with it.
		/// </summary>
		~JsonParseCoordinator();

		/// <summary>
		/// Duplicates the parse coordinator, including its wrapper and any helpers it has.
		/// </summary>
		/// <returns>Pointer to the cloned parse coordinator</returns>
		JsonParseCoordinator* Clone();

		JsonParseCoordinator(const JsonParseCoordinator& other) = delete;
		JsonParseCoordinator& operator=(const JsonParseCoordinator& other) = delete;

		/// <summary>
		/// Instantiates a parse coordinator from a preexisting parse coordinator, moving the contents over from the other parse coordinator
		/// </summary>
		/// <param name="other">Reference to the parse coordinator to move from</param>
		JsonParseCoordinator(JsonParseCoordinator&& other) noexcept;

		/// <summary>
		/// Instantiates a parse coordinator from a preexisting parse coordinator, moving the contents over from the other parse coordinator
		/// </summary>
		/// <param name="other">Reference to the parse coordinator to move from</param>
		/// <returns>Reference to the instantiated parse coordinator</returns>
		JsonParseCoordinator& operator=(JsonParseCoordinator&& other) noexcept;

		/// <summary>
		/// Initializes all helpers associated with the parse coordinator.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Cleans up all helpers associated with the parse coordinator.
		/// </summary>
		void Cleanup();

		/// <summary>
		/// If the parse coordinator is not a clone, adds the helper to the coordinator's list of helpers
		/// </summary>
		/// <param name="insertHelper">Helper to be added</param>
		/// <exception cref="std::runtime_error">
		/// Throws when a type of helper already exists with the parse coordinator
		/// </exception>
		void AddHelper(IJsonParseHelper& insertHelper);

		/// <summary>
		/// If the parse coordinator is not a clone, removes the helper from the coordinator's list of helpers.
		/// </summary>
		/// <param name="helper">Helper to be removed</param>
		void RemoveHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Parses the string using the helpers associated with the coordinator
		/// </summary>
		/// <param name="key">String to be parsed</param>
		void Parse(const std::string& key);

		/// <summary>
		/// Parses the file using the helpers associated with the coordinator and stores the file name
		/// </summary>
		/// <param name="filename">Name of the file to be parsed</param>
		void ParseFromFile(std::string filename);

		/// <summary>
		/// Parses the input stream using the helpers associated with the coordinator
		/// </summary>
		/// <param name="stream"></param>
		void Parse(std::istream& stream);

		/// <summary>
		/// Gets the file name associated with the parse coordinator
		/// </summary>
		/// <returns>Const reference to the file name</returns>
		const std::string& GetFileName() const;

		/// <summary>
		/// Gets the wrapper associated with the parse coordinator
		/// </summary>
		/// <returns>Reference to the wrapper</returns>
		Wrapper& GetWrapper();

		/// <summary>
		/// Gets the wrapper associated with the parse coordinator
		/// </summary>
		/// <returns>Reference to the wrapper</returns>
		const Wrapper& GetWrapper() const;

		/// <summary>
		/// Sets the wrapper to be associated with the parse coordinator
		/// </summary>
		/// <param name="wrapper">Wrapper to associate with the parse coordinator</param>
		void SetWrapper(Wrapper& wrapper);

		/// <summary>
		/// Returns if the parse coordinator is a clone or not
		/// </summary>
		/// <returns>True if the parse coordinator is a clone, false otherwise</returns>
		bool IsClone() const;

		/// <summary>
		/// Returns a reference to the helpers associated with the parse coordinator
		/// </summary>
		/// <returns>Reference to the helpers associated with the parse coordinator</returns>
		Vector<IJsonParseHelper*>& Helpers();
		
	private:
		void ParseMembers(const Json::Value& value);
		void Parse(const std::string& key, const Json::Value& value, size_t arrIndex);

		Vector<IJsonParseHelper*> _helpers;
		std::string _filename;
		Wrapper* _wrapper = nullptr;
		bool _isClone = false;
	};
}