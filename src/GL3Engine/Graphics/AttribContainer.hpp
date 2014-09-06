#ifndef ATTRIBCONTAINER_HPP_
#define ATTRIBCONTAINER_HPP_
#include "../Tools.hpp"

namespace GL3Engine {
    class ScopedContainer {
        public:
            virtual void begin() const = 0;
            virtual void end() const = 0;
            virtual ~ScopedContainer() {
            }
    };
    template<typename C>
    class AttribContainer {
        protected:
            deque<C> container;

        public:
            C attrib;

            AttribContainer() {
            }
            AttribContainer(C _attrib)
                    :
                      attrib(_attrib) {
            }

            virtual inline AttribContainer<C>& pushAttrib() {
                container.push_back(attrib);
                return *this;
            }
            virtual inline AttribContainer<C>& popAttrib() {
                attrib = container.back();
                container.pop_back();
                return *this;
            }

            C& getAttrib() {
                return attrib;
            }
            void setAttrib(const C& c) {
                attrib = c;
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
            deque<C>& getAttribContainer() const {
                return container;
            }

            virtual ~AttribContainer() {
            }
    };
}

#endif
