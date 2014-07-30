#ifndef ECS_HPP_
#define ECS_HPP_
#include <typeinfo>
#include <unordered_map>
#include <map>
#include <assert.h>
#include <algorithm>
#include <boost/any.hpp>
#include <typeindex>

#include "../Graphics/Types.hpp"

namespace GL3Engine {
    using namespace boost;
    extern void debugECS();

    class Component {
        public:
            virtual size_t getClassHash() const = 0;
            virtual ~Component() {
            }
    };
    using TypePtr = type_index;

#define RTII_REG_CLASS(type) \
    public: \
    size_t getClassHash() const { \
        static size_t hash = typeid(type).hash_code(); \
        return hash; \
    }
#define CHECK_CTYPE(type) \
    static_assert(is_base_of<Component, type>::value, "Bad Component type!")

    class Entity {
        private:
            unordered_map<size_t, unique_ptr<Component>> components;

        public:
            template<typename C> C* get() {
                size_t hash = typeid(C).hash_code();
                {
                    assert(IS_IN_MAP(components, hash));
                    return dynamic_cast<C*>(components[hash].get());
                }
            }

            template<typename T> inline GLboolean has() const {
                return IS_IN_MAP(components, typeid(T).hash_code());
            }
            inline GLboolean has(TypePtr ptr) const {
                return IS_IN_MAP(components, ptr.hash_code());
            }

            template<typename C> Entity& add() {
                CHECK_CTYPE(C);
                {
                    Component* c = new C;
                    components[c->getClassHash()] = c;
                }
                return *this;
            }
            template<typename C> Entity& add(C* c) {
                CHECK_CTYPE(C);
                {
                    assert(c != nullptr);
                    components[c->getClassHash()] = unique_ptr<Component>(c);
                }
                return *this;
            }
            template<typename ... C> inline Entity& add(C*... c) {
                return add(c...);
            }
    };
    class CSystem {
        private:
            vector<TypePtr> c_types;
            vector<Entity*> entities;

        public:
            CSystem(const vector<TypePtr>& _c_types)
                    :
                      c_types(_c_types) {
            }

            CSystem& delEntity(Entity* c) {
                entities.erase(
                        remove(entities.begin(), entities.end(), c),
                        entities.end());
                return *this;
            }
            CSystem& regEntity(Entity* c) {
                assert(c != nullptr);
                for (auto& ptr : c_types)
                    if (!c->has(ptr))
                        return *this;
                entities.push_back(c);
                return *this;
            }
            vector<Entity*>& getEntities() {
                return entities;
            }

            virtual void logic(Entity* c) = 0;
            virtual void update() {
                for (auto* c : entities)
                    logic(c);
            }

            virtual ~CSystem() {
            }
    };
    template<typename ... T> class CTSystem : public CSystem {
        public:
            CTSystem()
                    :
                      CSystem( { typeid(T)... }) {
            }
    };

    class CWorld : public Singleton<CWorld> {
            using SystemMap = map<TypePtr, unique_ptr<CSystem>>;
            using Entities = vector<unique_ptr<Entity>>;

        private:
            SystemMap systems;
            Entities entities;

        public:
            template<typename T> CWorld& regSystem() {
                static_assert(is_base_of<CSystem, T>::value, "Invalid system!");
                systems.insert(
                        make_pair<TypePtr, unique_ptr<CSystem>>(
                                typeid(T),
                                unique_ptr<CSystem>(new T)));
                return *this;
            }
            template<typename T> T* getSystem() {
                {
                    static_assert(is_base_of<CSystem, T>::value, "System not exists!");
                    assert(IS_IN_MAP(systems, typeid(T)));
                }
                return dynamic_cast<T*>(systems[typeid(T)].get());
            }

            CWorld& regEntity(Entity* e) {
                assert(e);
                {
                    entities.push_back(unique_ptr < Entity > (e));
                    for (auto& sys : systems)
                        sys.second->regEntity(e);
                }
                return *this;
            }
            CWorld& delEntity(Entity* e) {
                assert(e);
                {
                    for (auto& sys : systems)
                        sys.second->delEntity(e);
                    auto p = remove_if(entities.begin(), entities.end(),
                            [e](const unique_ptr<Entity>& v)
                            {
                                return v.get() == e;
                            });
                    entities.erase(p, entities.end());
                }
                return *this;
            }

            inline void update() {
                for (auto& sys : systems)
                    sys.second->update();
            }
    };

    template<typename T> Component* createInstance() {
        return dynamic_cast<Component*>(new T);
    }
    class CTypeManager : public Singleton<CTypeManager> {
        protected:
            using C_INSTANCE_PTR = Component*(*)();
            map<string, C_INSTANCE_PTR> handles;

        public:
            template<typename T> CTypeManager& regCType(c_str name) {
                CHECK_CTYPE(T);
                {
                    handles[name] = &createInstance<T>;
                }
                return *this;
            }
            template<typename T = Component> inline T* operator[](c_str name) {
                if (!IS_IN_MAP(handles, name))
                    return nullptr;
                return dynamic_cast<T*>(handles[name]());
            }
    };

#define DECLARE_CTYPE(type) \
    struct C##type##InitBlock { \
        C##type##InitBlock() { CTypeManager::getInstance().regCType<type>(#type); } \
    }; \
    static C##type##InitBlock   __st_block_##type;
#define REQUIRE_CTYPE_OBJ(name) \
    CTypeManager::getInstance().operator[]<>(name)
}

#endif
