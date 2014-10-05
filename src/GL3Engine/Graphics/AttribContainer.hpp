#ifndef ATTRIBCONTAINER_HPP_
#define ATTRIBCONTAINER_HPP_
#include <ctime>

#include "../Tools.hpp"

namespace GL3Engine {
    namespace CoreAttrib {
        class ScopedContainer {
            public:
                virtual void begin() = 0;
                virtual void end() = 0;
                virtual ~ScopedContainer() {
                }
        };
        template<typename C, typename T> class AttribContainer {
            protected:
                std::deque<C> container;

            public:
                C attrib;

                AttribContainer() {
                }
                AttribContainer(C _attrib)
                        :
                          attrib(_attrib) {
                }

                virtual inline AttribContainer<C, T>& pushAttrib() {
                    container.push_back(attrib);
                    return *this;
                }
                virtual inline AttribContainer<C, T>& popAttrib() {
                    attrib = container.back();
                    container.pop_back();
                    return *this;
                }

                C& getAttrib() {
                    return attrib;
                }
                T& setAttrib(const C& c) {
                    attrib = c;
                    return dynamic_cast<T&>(*this);
                }

                C operator->() {
                    return attrib;
                }
                operator C() {
                    return attrib;
                }
                //            inline C& get() {
                //                return attrib;
                //            }
                std::deque<C>& getAttribContainer() const {
                    return container;
                }

                virtual ~AttribContainer() {
                }
        };
    }
    namespace CoreUtils {
        class Profiler :
                         public CoreAttrib::ScopedContainer,
                         public CoreInterface::Singleton<Profiler> {
            private:
                std::clock_t time;

            public:
                void begin() override {
                    time = std::clock();
                }
                void end() override {
                    LOG(WARNING,
                            "Profiler:" +
                            std::to_string((std::clock() - time) / (GLdouble)(CLOCKS_PER_SEC / 1000))
                            + "ms");
                }
        };
    }
}

#endif
