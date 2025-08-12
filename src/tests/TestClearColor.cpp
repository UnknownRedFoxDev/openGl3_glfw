#include "TestClearColor.h"
#include "Renderer.h"
#include "ImGui/imgui.h"

namespace test {

    TestClearColor::TestClearColor() : mClearColor { 0.2f, 0.3f, 0.8f, 1.0f } {
    }
    TestClearColor::~TestClearColor() {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestClearColor::OnUpdate() {
        GLCall(glClearColor(mClearColor[0],mClearColor[1],mClearColor[2],mClearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
    void TestClearColor::OnRender() {
    }
    void TestClearColor::OnImGuiRender() {
        ImGui::ColorEdit4("Clear Color", mClearColor);
    }
}
