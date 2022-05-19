#pragma once
#include "pch.h"
#include "Datum.h"

namespace FieaGameEngine
{
	class Signature final
	{
	public:
		/// <summary>
		/// Constructor of Signature
		/// </summary>
		/// <param name="name">the name(std::string) of the signature</param>
		/// <param name="type">the type(DatumTypes) of the signature</param>
		/// <param name="size">the size of the signature</param>
		/// <param name="offset">the offset of the signature</param>
		Signature(std::string name, DatumTypes type, std::size_t size, std::size_t offset);

		std::string _name;
		DatumTypes _type;
		std::size_t _size;
		std::size_t _offset;
	};
}