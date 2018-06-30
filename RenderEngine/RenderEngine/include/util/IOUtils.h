/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

namespace Engine
{
	/**
	 * I/O utilities
	 */
	namespace IO
	{
		char * loadStringFromFile(const char * fileName, unsigned long long & fileLen);
	}
}