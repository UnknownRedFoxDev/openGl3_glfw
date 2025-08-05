#pragma once

#include "Test.h"

namespace test {

	class TestClearColor : public Test {
		public:
			TestClearColor();
			~TestClearColor();

			void OnUpdate(float deltaTime);
			void OnRender();
			void OnImGuiRender();
		private:
			float mClearColor[4];
	};

}
