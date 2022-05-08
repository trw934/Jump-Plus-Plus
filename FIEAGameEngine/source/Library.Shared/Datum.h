#pragma once
#include <cstdint>
#include <string>
#include <cstddef>
#include <stdexcept>
#include <cassert>
#include "RTTI.h"
#include "glm\glm.hpp"

namespace FieaGameEngine
{
	enum class DatumTypes
	{
		Unknown,
		Integer,
		Float,
		Vector,
		Matrix,
		String,
		Pointer,
		Table,

		Begin = Integer,
		End = Table
	};

	class Scope;

	class Datum final
	{
		friend class Scope;
		friend class Attributed;

	public:
		/// <summary>
		/// Defaultly constructs an empty datum with Unknown type
		/// </summary>
		Datum() = default;

		/// <summary>
		/// Constructs a Integer Datum using the provided parameter
		/// </summary>
		/// <param name="value">integer to set the datum to</param>
		/// <returns>reference to the datum after assignment</returns>
		Datum& operator=(const std::int32_t& value);

		/// <summary>
		/// Constructs a Float Datum using the provided parameter
		/// </summary>
		/// <param name="value">float to set the datum to</param>
		/// <returns>reference to the datum after assignment</returns>
		Datum& operator=(const float& value);

		/// <summary>
		/// Constructs a Vector Datum using the provided parameter
		/// </summary>
		/// <param name="value">glm::vec4 to set the datum to</param>
		/// <returns>reference to the datum after assignment</returns>
		Datum& operator=(const glm::vec4& value);

		/// <summary>
		/// Constructs a Matrix Datum using the provided parameter
		/// </summary>
		/// <param name="value">glm::mat4 to set the datum to</param>
		/// <returns>reference to the datum after assignment</returns>
		Datum& operator=(const glm::mat4& value);

		/// <summary>
		/// Constructs a String Datum using the provided parameter
		/// </summary>
		/// <param name="value">string to set the datum to</param>
		/// <returns>reference to the datum after assignment</returns>
		Datum& operator=(const std::string& value);

		/// <summary>
		/// Constructs a Pointer Datum using the provided parameter
		/// </summary>
		/// <param name="value">RTTI pointer to set the datum to</param>
		/// <returns>reference to the datum after assignment</returns>
		Datum& operator=(FieaGameEngine::RTTI* const& value);

		

		/// <summary>
		/// Instantiates the datum from an existing datum
		/// </summary>
		/// <param name="other">A reference to the datum to copy over</param>
		Datum(const Datum& other);

		/// <summary>
		/// Instantiates a datum from an existing datum
		/// </summary>
		/// <param name="other">A reference to the datum to copy over</param>
		/// <returns>A reference to the instantiated datum</returns>
		Datum& operator=(const Datum& other);

		/// <summary>
		/// Instantiates a datum from an existing datum, moving the contents over from the other datum
		/// </summary>
		/// <param name="other">A reference to the datum to move over</param>
		Datum(Datum&& other);

		/// <summary>
		/// Instantiates a datum from an existing datum, moving the contents over from the other datum
		/// </summary>
		/// <param name="other">A reference to the datum to move over</param>
		/// <returns>A reference to the instantiated datum</returns>
		Datum& operator=(Datum&& other);

		/// <summary>
		/// Destroys a datum
		/// </summary>
		~Datum();

		/// <summary>
		/// Compares the contents and fields of the datum to the datum passed in
		/// </summary>
		/// <param name="other">A reference to the datum to be compared</param>
		/// <returns>Returns true if the datums have the same size, capacity, type, and data. Returns false otherwise</returns>
		bool operator==(const Datum& other) const;

		/// <summary>
		/// Compares the contents and fields of the datum to the datum passed in
		/// </summary>
		/// <param name="other">A reference to the datum to be compared</param>
		/// <returns>Returns false if the datums have the same size, capacity, type, and data. Returns true otherwise</returns>
		bool operator!=(const Datum& other) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the integer passed in</param>
		/// <returns>Returns true if the first element of the datum is equivalent to the parameter, false otherwise</returns>
		bool operator==(std::int32_t value) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the integer passed in</param>
		/// <returns>Returns false if the first element of the datum is equivalent to the parameter, true otherwise</returns>
		bool operator!=(std::int32_t value) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the float passed in</param>
		/// <returns>Returns true if the first element of the datum is equivalent to the parameter, false otherwise</returns>
		bool operator==(float value) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the float passed in</param>
		/// <returns>Returns false if the first element of the datum is equivalent to the parameter, true otherwise</returns>
		bool operator!=(float value) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the glm::vec4 passed in</param>
		/// <returns>Returns true if the first element of the datum is equivalent to the parameter, false otherwise</returns>
		bool operator==(const glm::vec4& value) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the glm::vec4 passed in</param>
		/// <returns>Returns false if the first element of the datum is equivalent to the parameter, true otherwise</returns>
		bool operator!=(const glm::vec4& value) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the glm::mat4 passed in</param>
		/// <returns>Returns true if the first element of the datum is equivalent to the parameter, false otherwise</returns>
		bool operator==(const glm::mat4& value) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the glm::mat4 passed in</param>
		/// <returns>Returns false if the first element of the datum is equivalent to the parameter, true otherwise</returns>
		bool operator!=(const glm::mat4& value) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the string passed in</param>
		/// <returns>Returns true if the first element of the datum is equivalent to the parameter, false otherwise</returns>
		bool operator==(const std::string& value) const;
		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the string passed in</param>
		/// <returns>Returns false if the first element of the datum is equivalent to the parameter, true otherwise</returns>
		bool operator!=(const std::string& value) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the RTTI pointer passed in</param>
		/// <returns>Returns true if the first element of the datum is equivalent to the parameter, false otherwise</returns>
		bool operator==(FieaGameEngine::RTTI* const& value) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the RTTI pointer passed in</param>
		/// <returns>Returns false if the first element of the datum is equivalent to the parameter, true otherwise</returns>
		bool operator!=(FieaGameEngine::RTTI* const& value) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the Scope pointer passed in</param>
		/// <returns>Returns true if the first element of the datum is equivalent to the parameter, false otherwise</returns>
		bool operator==(FieaGameEngine::Scope* const& value) const;

		/// <summary>
		/// Compares the first element of the datum to the value passed in
		/// </summary>
		/// <param name="other">A reference to the Scope pointer passed in</param>
		/// <returns>Returns false if the first element of the datum is equivalent to the parameter, true otherwise</returns>
		bool operator!=(FieaGameEngine::Scope* const& value) const;

		/// <summary>
		/// Returns the enum of the type of the datum
		/// </summary>
		/// <returns>The type of the datum</returns>
		DatumTypes Type() const;

		/// <summary>
		/// Sets the type of the datum if it hasn't already been set
		/// </summary>
		/// <param name="type">The type the datum is to be set to</param>
		/// <exception cref="std::runtime_error">
		/// Throws when type has already been set or if external storage.
		/// </exception>
		void SetType(DatumTypes type);

		/// <summary>
		/// Returns the size of the datum
		/// </summary>
		/// <returns>The size of the datum</returns>
		std::size_t Size() const;

		/// <summary>
		/// Returns the capacity of the datum
		/// </summary>
		/// <returns>The capacity of the datum</returns>
		std::size_t Capacity() const;
		
		/// <summary>
		/// Adds more capacity to the datum. The new capacity must be larger than the current capacity
		/// </summary>
		/// <param name="capacity">The desired capacity</param>
		/// <exception cref="std::runtime_error">
		/// Throws when type is Unknown or if external storage
		/// </exception>
		void Reserve(std::size_t capacity);

		/// <summary>
		/// Resizes the datum to the desired size, default constructing elements if increasing size and destructing elements if
		/// decreasing size. The capacity will be equivalent to the size after calling this method.
		/// </summary>
		/// <param name="size">The desired size</param>
		/// <exception cref="std::runtime_error">
		/// Throws when type is Unknown or if external storage
		/// </exception>
		void Resize(std::size_t size);

		/// <summary>
		/// Lowers the capacity of the datum to be equivalent to the current size.
		/// </summary>
		/// <exception cref="std::runtime_error">
		/// Throws when type is Unknown or if external storage
		/// </exception>
		void ShrinkToFit();

		/// <summary>
		/// Clears the contents of the datum without shrinking capacity
		/// </summary>
		/// <exception cref="std::runtime_error">
		/// Throws when datum points to external storage
		/// </exception>
		void Clear();

		/// <summary>
		/// Assigns external storage to the datum. Cannot set datum that has already been allocated memory to external storage.
		/// Datums set to external storage cannot modify the data.
		/// </summary>
		/// <param name="values">Array of integers to set the datum to</param>
		/// <param name="numElements">Number of elements in the given array</param>
		/// <exception cref="std::runtime_error">
		/// Throws when datum already has capacity or a type, or if external storage
		/// </exception>
		void SetStorage(std::int32_t* values, std::size_t numElements);

		/// <summary>
		/// Assigns external storage to the datum. Cannot set datum that has already been allocated memory to external storage.
		/// Datums set to external storage cannot modify the data.
		/// </summary>
		/// <param name="values">Array of floats to set the datum to</param>
		/// <param name="numElements">Number of elements in the given array</param>
		/// <exception cref="std::runtime_error">
		/// Throws when datum already has capacity or a type, or if external storage
		/// </exception>
		void SetStorage(float* values, std::size_t numElements);

		/// <summary>
		/// Assigns external storage to the datum. Cannot set datum that has already been allocated memory to external storage.
		/// Datums set to external storage cannot modify the data.
		/// </summary>
		/// <param name="values">Array of glm::vec4s to set the datum to</param>
		/// <param name="numElements">Number of elements in the given array</param>
		/// <exception cref="std::runtime_error">
		/// Throws when datum already has capacity or a type, or if external storage
		/// </exception>
		void SetStorage(glm::vec4* values, std::size_t numElements);

		/// <summary>
		/// Assigns external storage to the datum. Cannot set datum that has already been allocated memory to external storage.
		/// Datums set to external storage cannot modify the data.
		/// </summary>
		/// <param name="values">Array of glm::mat4s to set the datum to</param>
		/// <param name="numElements">Number of elements in the given array</param>
		/// <exception cref="std::runtime_error">
		/// Throws when datum already has capacity or a type, or if external storage
		/// </exception>
		void SetStorage(glm::mat4* values, std::size_t numElements);

		/// <summary>
		/// Assigns external storage to the datum. Cannot set datum that has already been allocated memory to external storage.
		/// Datums set to external storage cannot modify the data.
		/// </summary>
		/// <param name="values">Array of strings to set the datum to</param>
		/// <param name="numElements">Number of elements in the given array</param>
		/// <exception cref="std::runtime_error">
		/// Throws when datum already has capacity or a type, or if external storage
		/// </exception>
		void SetStorage(std::string *values, std::size_t numElements);

		/// <summary>
		/// Assigns external storage to the datum. Cannot set datum that has already been allocated memory to external storage.
		/// Datums set to external storage cannot modify the size of the data.
		/// </summary>
		/// <param name="values">Array of RTTI pointers to set the datum to</param>
		/// <param name="numElements">Number of elements in the given array</param>
		/// <exception cref="std::runtime_error">
		/// Throws when datum already has capacity or a type, or if external storage
		/// </exception>
		void SetStorage(FieaGameEngine::RTTI** values, std::size_t numElements);

		/// <summary>
		/// Sets the data at the given index to the given value
		/// </summary>
		/// <param name="value">Integer to set the data to</param>
		/// <param name="index">Index of the data to be set</param>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Integer
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		void Set(const std::int32_t value, std::size_t index = 0);

		/// <summary>
		/// Sets the data at the given index to the given value
		/// </summary>
		/// <param name="value">Float to set the data to</param>
		/// <param name="index">Index of the data to be set</param>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Float
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		void Set(const float value, std::size_t index = 0);

		/// <summary>
		/// Sets the data at the given index to the given value
		/// </summary>
		/// <param name="value">glm::vec4 to set the data to</param>
		/// <param name="index">Index of the data to be set</param>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Vector
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		void Set(const glm::vec4& value, std::size_t index = 0);

		/// <summary>
		/// Sets the data at the given index to the given value
		/// </summary>
		/// <param name="value">glm::mat4 to set the data to</param>
		/// <param name="index">Index of the data to be set</param>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Matrix
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		void Set(const glm::mat4& value, std::size_t index = 0);

		/// <summary>
		/// Sets the data at the given index to the given value
		/// </summary>
		/// <param name="value">String to set the data to</param>
		/// <param name="index">Index of the data to be set</param>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not String
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		void Set(const std::string& value, std::size_t index = 0);

		/// <summary>
		/// Sets the data at the given index to the given value
		/// </summary>
		/// <param name="value">RTTI pointer to set the data to</param>
		/// <param name="index">Index of the data to be set</param>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Pointer
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		void Set(FieaGameEngine::RTTI* const& value, std::size_t index = 0);

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>Reference to the integer that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Integer
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		std::int32_t& GetInt(std::size_t index = 0);

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>const Reference to the integer that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Integer
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		const std::int32_t GetInt(std::size_t index = 0) const;

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>Reference to the float that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Float
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		float& GetFloat(std::size_t index = 0);

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>const Reference to the float that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Float
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		const float GetFloat(std::size_t index = 0) const;

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>Reference to the glm::vec4 that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Vector
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		glm::vec4& GetVector(std::size_t index = 0);

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>const Reference to the glm::vec4 that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Vector
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		const glm::vec4& GetVector(std::size_t index = 0) const;

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>Reference to the glm::mat4 that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Matrix
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		glm::mat4& GetMatrix(std::size_t index = 0);

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>const Reference to the glm::mat4 that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Matrix
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		const glm::mat4& GetMatrix(std::size_t index = 0) const;

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>Reference to the string that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not String
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		std::string& GetString(std::size_t index = 0);

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>const Reference to the string that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not String
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		const std::string& GetString(std::size_t index = 0) const;

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>Reference to the RTTI pointer that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Pointer
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		FieaGameEngine::RTTI*& GetPointer(std::size_t index = 0);

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>const Reference to the RTTI pointer that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Pointer
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		FieaGameEngine::RTTI* GetPointer(std::size_t index = 0) const;

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>Reference to the Scope pointer that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Pointer
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		FieaGameEngine::Scope*& GetTable(std::size_t index = 0);

		/// <summary>
		/// Retrieves a reference to the data at the specified index
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>const Reference to the Scope pointer that was retrieved</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Pointer
		/// </exception>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		FieaGameEngine::Scope* GetTable(std::size_t index = 0) const;

		/// <summary>
		/// Calls PushBack after converting the value from a string.
		/// </summary>
		/// <param name="string">The string representing the value to be entered</param>
		/// <param name="index">Index of the data to be set</param>
		void PushBackFromString(const std::string& value);

		/// <summary>
		/// Sets the data at the specified index to the value parsed from the string
		/// </summary>
		/// <param name="string">The string representing the value to be entered</param>
		/// <param name="index">Index of the data to be set</param>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		/// <exception cref="std::runtime_error">
		/// Throws when string cannot be converted to correct data type or datum has Unknown type
		/// </exception>
		void SetFromString(const std::string& value, std::size_t index = 0);

		/// <summary>
		/// Retrieves the value at the specified index and converts it into a string
		/// </summary>
		/// <param name="index">Index of the data to be retrieved</param>
		/// <returns>A string representation of the data at the index</returns>
		/// <exception cref="std::out_of_range">
		/// Throws when index >= size of datum
		/// </exception>
		/// <exception cref="std::runtime_error">
		/// Throws when datum has Unknown type
		/// </exception>
		std::string ToString(std::size_t index = 0);

		/// <summary>
		/// Appends given value at the end of the datum. Doubles capacity if the new size would exceed original capacity
		/// </summary>
		/// <param name="data">Integer to be inserted</param>
		/// <exception cref="std::runtime_error">
		/// Throws when value doesn't match datum type or external storage
		/// </exception>
		void PushBack(std::int32_t value);

		/// <summary>
		/// Appends given value at the end of the datum. Doubles capacity if the new size would exceed original capacity
		/// </summary>
		/// <param name="data">Float to be inserted</param>
		/// <exception cref="std::runtime_error">
		/// Throws when value doesn't match datum type or external storage
		/// </exception>
		void PushBack(float value);

		/// <summary>
		/// Appends given value at the end of the datum. Doubles capacity if the new size would exceed original capacity
		/// </summary>
		/// <param name="data">const reference to the glm::vec4 to be inserted</param>
		/// <exception cref="std::runtime_error">
		/// Throws when value doesn't match datum type or external storage
		/// </exception>
		void PushBack(const glm::vec4& value);

		/// <summary>
		/// Appends given value at the end of the datum. Doubles capacity if the new size would exceed original capacity
		/// </summary>
		/// <param name="data">const reference to the glm::mat4 to be inserted</param>
		/// <exception cref="std::runtime_error">
		/// Throws when value doesn't match datum type or external storage
		/// </exception>
		void PushBack(const glm::mat4& value);

		/// <summary>
		/// Appends given value at the end of the datum. Doubles capacity if the new size would exceed original capacity
		/// </summary>
		/// <param name="data">const reference to the string to be inserted</param>
		/// <exception cref="std::runtime_error">
		/// Throws when value doesn't match datum type or external storage
		/// </exception>
		void PushBack(const std::string& value);

		/// <summary>
		/// Appends given value at the end of the datum. Doubles capacity if the new size would exceed original capacity
		/// </summary>
		/// <param name="data">const reference to the RTTI pointer to be inserted</param>
		/// <exception cref="std::runtime_error">
		/// Throws when value doesn't match datum type or external storage
		/// </exception>
		void PushBack(FieaGameEngine::RTTI* const & value);

		/// <summary>
		/// Removes the last item of the datum
		/// </summary>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or external storage
		/// </exception>
		void PopBack();

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>Reference to the integer at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		std::int32_t& FrontInt();

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>const Reference to the integer at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		const std::int32_t& FrontInt() const;

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>Reference to the float at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		float& FrontFloat();

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>const Reference to the integer at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		const float& FrontFloat() const;

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>Reference to the glm::vec4 at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		glm::vec4& FrontVector();

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>const Reference to the glm::vec4 at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		const glm::vec4& FrontVector() const;

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>Reference to the glm::mat4 at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		glm::mat4& FrontMatrix();

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>const Reference to the glm::mat4 at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		const glm::mat4& FrontMatrix() const;

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>Reference to the string at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		std::string& FrontString();

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>const Reference to the string at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		const std::string& FrontString() const;

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>Reference to the RTTI pointer at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		FieaGameEngine::RTTI*& FrontPointer();

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>const Reference to the RTTI pointer at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		FieaGameEngine::RTTI* const & FrontPointer() const;

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>Reference to the Scope pointer at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		FieaGameEngine::Scope*& FrontTable();

		/// <summary>
		/// Returns the value at the front of the datum
		/// </summary>
		/// <returns>const Reference to the Scope pointer at the front of the datum</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		FieaGameEngine::Scope* const& FrontTable() const;

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>Reference to the integer at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		std::int32_t& BackInt();

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>const Reference to the integer at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		const std::int32_t& BackInt() const;

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>Reference to the float at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		float& BackFloat();

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>const Reference to the float at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		const float& BackFloat() const;

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>Reference to the glm::vec4 at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		glm::vec4& BackVector();

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>const Reference to the glm::vec4 at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		const glm::vec4& BackVector() const;

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>Reference to the glm::mat4 at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		glm::mat4& BackMatrix();

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>const Reference to the glm::mat4 at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		const glm::mat4& BackMatrix() const;

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>Reference to the string at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		std::string& BackString();

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>const Reference to the string at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		const std::string& BackString() const;

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>Reference to the RTTI pointer at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		FieaGameEngine::RTTI*& BackPointer();

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>const Reference to the RTTI pointer at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		FieaGameEngine::RTTI* const& BackPointer() const;

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>Reference to the RTTI pointer at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		FieaGameEngine::Scope*& BackTable();

		/// <summary>
		/// Returns the value at the back of the datum
		/// </summary>
		/// <returns>const Reference to the RTTI pointer at the back of the datum></returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum size is 0 or value doesn't match datum type
		/// </exception>
		FieaGameEngine::Scope* const& BackTable() const;

		/// <summary>
		/// Searches for the given value in the contents of the datum
		/// </summary>
		/// <param name="value">Integer to search for</param>
		/// <returns>
		/// Pair of <bool, size_t>. bool is true if value is found, false otherwise.
		/// Size_t will be equal to the index of the value if found, _size otherwise.
		/// </returns>
		std::pair<bool, std::size_t> Find(const std::int32_t& value) const;

		/// <summary>
		/// Searches for the given value in the contents of the datum
		/// </summary>
		/// <param name="value">Float to search for</param>
		/// <returns>
		/// Pair of <bool, size_t>. bool is true if value is found, false otherwise.
		/// Size_t will be equal to the index of the value if found, _size otherwise.
		/// </returns>
		std::pair<bool, std::size_t> Find(const float& value) const;

		/// <summary>
		/// Searches for the given value in the contents of the datum
		/// </summary>
		/// <param name="value">glm::vec4 to search for</param>
		/// <returns>
		/// Pair of <bool, size_t>. bool is true if value is found, false otherwise.
		/// Size_t will be equal to the index of the value if found, _size otherwise.
		/// </returns>
		std::pair<bool, std::size_t> Find(const glm::vec4& value) const;

		/// <summary>
		/// Searches for the given value in the contents of the datum
		/// </summary>
		/// <param name="value">glm::mat4 to search for</param>
		/// <returns>
		/// Pair of <bool, size_t>. bool is true if value is found, false otherwise.
		/// Size_t will be equal to the index of the value if found, _size otherwise.
		/// </returns>
		std::pair<bool, std::size_t> Find(const glm::mat4& value) const;

		/// <summary>
		/// Searches for the given value in the contents of the datum
		/// </summary>
		/// <param name="value">string to search for</param>
		/// <returns>
		/// Pair of <bool, size_t>. bool is true if value is found, false otherwise.
		/// Size_t will be equal to the index of the value if found, _size otherwise.
		/// </returns>
		std::pair<bool, std::size_t> Find(const std::string& value) const;

		/// <summary>
		/// Searches for the given value in the contents of the datum
		/// </summary>
		/// <param name="value">RTTI pointer to search for</param>
		/// <returns>
		/// Pair of <bool, size_t>. bool is true if value is found, false otherwise.
		/// Size_t will be equal to the index of the value if found, _size otherwise.
		/// </returns>
		std::pair<bool, std::size_t> Find(FieaGameEngine::RTTI* const& value) const;

		/// <summary>
		/// Searches for the given value in the contents of the datum
		/// </summary>
		/// <param name="value">Scope pointer to search for</param>
		/// <returns>
		/// Pair of <bool, size_t>. bool is true if value is found, false otherwise.
		/// Size_t will be equal to the index of the value if found, _size otherwise.
		/// </returns>
		std::pair<bool, std::size_t> Find(FieaGameEngine::Scope& value) const;

		/// <summary>
		/// Searches for the given value in the contents of the datum
		/// </summary>
		/// <param name="value">Scope pointer to search for</param>
		/// <returns>
		/// Pair of <bool, size_t>. bool is true if value is found, false otherwise.
		/// Size_t will be equal to the index of the value if found, _size otherwise.
		/// </returns>
		std::pair<bool, std::size_t> Find(const FieaGameEngine::Scope& value) const;

		/// <summary>
		/// Removes the value from the vector at the given index. The contents of the array will shift down an index to fill the gap left by deleting the element
		/// </summary>
		/// <param name="index">An index to the value to be removed</param>
		/// <returns>true if removal was successful, false otherwise</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum points to external storage
		/// </exception>
		bool RemoveAt(std::size_t index);

		/// <summary>
		/// Searches for the value within the datum and removes it if it is found.
		/// </summary>
		/// <param name="value">Integer to remove</param>
		/// <returns>True if removal was successful, false otherwise</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum points to external storage
		/// </exception>
		bool Remove(const std::int32_t& value);

		/// <summary>
		/// Searches for the value within the datum and removes it if it is found.
		/// </summary>
		/// <param name="value">Float to remove</param>
		/// <returns>True if removal was successful, false otherwise</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum points to external storage
		/// </exception>
		bool Remove(const float& value);

		/// <summary>
		/// Searches for the value within the datum and removes it if it is found.
		/// </summary>
		/// <param name="value">glm::vec4 to remove</param>
		/// <returns>True if removal was successful, false otherwise</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum points to external storage
		/// </exception>
		bool Remove(const glm::vec4& value);

		/// <summary>
		/// Searches for the value within the datum and removes it if it is found.
		/// </summary>
		/// <param name="value">glm::mat4 to remove</param>
		/// <returns>True if removal was successful, false otherwise</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum points to external storage
		/// </exception>
		bool Remove(const glm::mat4& value);

		/// <summary>
		/// Searches for the value within the datum and removes it if it is found.
		/// </summary>
		/// <param name="value">string to remove</param>
		/// <returns>True if removal was successful, false otherwise</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum points to external storage
		/// </exception>
		bool Remove(const std::string& value);

		/// <summary>
		/// Searches for the value within the datum and removes it if it is found.
		/// </summary>
		/// <param name="value">RTTI pointer to remove</param>
		/// <returns>True if removal was successful, false otherwise</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum points to external storage
		/// </exception>
		bool Remove(FieaGameEngine::RTTI* const& value);

		/// <summary>
		/// Returns a reference to the scope at the specified index
		/// </summary>
		/// <param name="index">Index of datum to grab</param>
		/// <returns>Reference to the scope</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Table
		/// </exception>
		Scope& operator[](std::size_t index);

		/// <summary>
		/// Returns a reference to the scope at the specified index
		/// </summary>
		/// <param name="index">Index of datum to grab</param>
		/// <returns>Reference to the scope</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when datum type is not Table
		/// </exception>
		Scope& operator[](std::size_t index) const;

		/// <summary>
		/// Checks if the datum is external or not
		/// </summary>
		/// <returns>True if datum is external, false if not</returns>
		bool IsExternal() const { return _isExternal; };

	private:
		Datum& operator=(FieaGameEngine::Scope& value);
		void SetStorage(DatumTypes type, uint8_t* values, std::size_t numElements);
		void Set(FieaGameEngine::Scope& value, std::size_t index = 0);
		void PushBack(FieaGameEngine::Scope& value);
		bool Remove(FieaGameEngine::Scope& value);

		union DatumValues final
		{
			std::uint8_t* b;
			std::int32_t* i;
			float* f;
			std::string* s;
			FieaGameEngine::RTTI** r;
			glm::vec4* v;
			glm::mat4* m;
			FieaGameEngine::Scope** t;
			void* vp{ nullptr };
		};

		DatumValues _data{ nullptr };
		DatumTypes _type{ DatumTypes::Unknown };
		std::size_t _capacity{ 0 };
		std::size_t _size{ 0 };
		bool _isExternal{ false };//if external, can only get and set
		static const std::size_t DatumTypeSizes [static_cast<std::size_t>((DatumTypes::End)) + 1];
	};
}
