#pragma once

#include "Test.h"

namespace test {

    class TestClearColor : public Test {
        public:
            TestClearColor();
            ~TestClearColor();

            void OnUpdate();
            void OnRender() override;
            void OnImGuiRender() override;
        private:
            float mClearColor[4];
    };

}
