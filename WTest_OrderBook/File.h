#pragma once
#ifndef  _WTEST_FILE_H
#define _WTEST_FILE_H

#include <string>
#include <fstream>
#include <cstddef>
#include "OrderItem.h"

namespace WG_ORDERBOOK
{
	using namespace std;

	/// <summary> Simple wrapper for a file. </summary>
	class orders_file
	{
		ifstream _stream; /// <summary> Stream to read. </summary>

	public:

		/// <summary> Dtor. </summary>
		~orders_file() noexcept
		{
			close();
		}

		/// <summary> Close file if opened. </summary>
		void close() noexcept
		{
			if (is_opened())
			{
				_stream.close();
			}
		}

		/// <summary> Opens file for read. </summary>
		/// <param name="file_name"> File name to open. </param>
		void open_for_read(const char* file_name)
		{
			close();

			_stream.open(file_name);
			if (!is_opened())
			{
				throw exception("Cannot open file.");
			}
		}

		/// <summary> Reads and validate orders records. </summary>
		/// <param name="record"> Reference to a record to write data in. </param>
		/// <returns> Returns false if end of file reached, true otherwise. </returns>
		bool read_order_record(order_record& record)
		{
			if (!is_opened())
			{
				throw exception("File is not opened.");
			}

			if (_stream.eof())
			{
				return false;
			}
			else
			{
				if (!(_stream >> record.timestamp))
				{
					throw exception("Invalid file format.");
				}

				char type;

				if (!(_stream >> type) || (type != 'I' && type != 'E'))
				{
					throw exception("Invalid file format.");
				}

				record.is_insert = type == 'I';

				if (!(_stream >> record.id))
				{
					throw exception("Invalid file format.");
				}

				if (record.is_insert)
				{
					if (!(_stream >> record.price))
					{
						throw exception("Invalid file format.");
					}
				}
				else
				{
					record.price = 0;
				}

				record.validate();

				return true;
			}
		}

		/// <summary> Checks if file is opened. </summary>
		/// <returns> Returns true if the file is in opened state. </returns>
		bool const is_opened() noexcept
		{
			return _stream.is_open();
		}
	};
}

#endif // ! _WTEST_FILE_H