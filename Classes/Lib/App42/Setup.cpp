#include "WkCocos/App42/Setup.h"

namespace WkCocos
{
	namespace App42
	{
		Setup::Setup(std::string APP_KEY, std::string SECRET_KEY)
		{
			::App42::App42API::Initialize(APP_KEY, SECRET_KEY);

		}

		Setup::~Setup()
		{
		}

	}// namespace App42
} //namespace WkCocos

