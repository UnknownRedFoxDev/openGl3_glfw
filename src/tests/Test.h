#pragma once

#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <functional>
#include <iostream>

namespace test {

    class Test {
        public:
            Test() {}
            virtual ~Test() {}

            virtual void OnUpdate() {}
            virtual void OnRender() {}
            virtual void OnImGuiRender() {}
    };

    class TestMenu : public Test {
        public:
            TestMenu(Test*& currentTestPointer);

            void OnImGuiRender() override;


            template<typename T, typename... Args>
                void RegisterTest(const std::string& name, Args&&... args) {
                    std::cout << "registering test: " << name << std::endl;
                    m_Tests.push_back(std::make_pair(name,
                        std::function<Test*()>([args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                            return std::apply([](auto&&... params) { return new T(std::forward<decltype(params)>(params)...); },
                                              std::move(args));
                        })));
                }


        private:
            Test*& m_CurrentTest;
            std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
    };
}
