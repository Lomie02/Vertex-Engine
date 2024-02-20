#pragma once

namespace UltraLight {

	class uApplication
	{
	public:

		// Constructor & de-constructor.
		uApplication();
		~uApplication();

		// Regular System loop functions.
		void StartUp();
		void UpdateSystems();

		bool IsApplicationRunning() { return m_IsApplicationRunning; }

	protected:

		bool m_IsApplicationRunning = true;
	};
}
