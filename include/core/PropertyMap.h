//
// Created by geislerd on 08.02.17.
//


#ifndef PROPERTYMAP_H
#define PROPERTYMAP_H

#include <string>
#include <boost/unordered_map.hpp>
#include <utils/Error.h>

namespace saliency_sandbox {

    namespace core {

        template<typename _type>
        class _PropertyMapContainer {
        protected:
            boost::unordered::unordered_map<std::string,_type> m_map;
        };

        template<typename _type, typename... _type_next>
        class _PropertyMapChain : protected _PropertyMapContainer<_type>, protected _PropertyMapChain<_type_next...> {

        };

        template<typename _type>
        class _PropertyMapChain<_type> { };

        template <typename... _types>
        class _PropertyMap : protected _PropertyMapChain< _types...>  {
        private:
            template<typename _type>
            using m = _PropertyMapContainer<_type>;

            template<typename t, typename... t_next>
            void _clr() {
                this->clr<t>();
                if(sizeof...(t_next) > 0)
                    this->_clr<t_next...>();
            };

        public:
            template<typename t>
            bool has(std::string name) {
                return this->m<t>::m_map.find(name) != this->m<t>::m_map.end();
            }

            template<typename t>
            t& get(std::string name) {
                sserr << sscond(!this->has<t>(name)) << "No property found with name \"" << name << "\n of type \"" << typeid(t).name() << "\"" << ssthrow;

                return this->m<t>::m_map[name];
            }

            template<typename t>
            t& get(std::string name, t value) {
                if(this->has<t>(name))
                    return this->m<t>::m_map[name];
                else {
                    this->m<t>::m_map[name] = value;
                    return this->m<t>::m_map[name];
                }

            }

            template<typename t>
            t& set(std::string name, t value) {
                this->m<t>::m_map[name] = value;
                return this->m<t>::m_map[name];
            }

            template<typename t>
            void del(std::string name) {
                sserr << sscond(!this->has<t>(name)) << "No property found with name \"" << name << "\n of type \"" << typeid(t).name() << "\"" << ssthrow;

                this->m<t>::m_map.erase(name);
            }

            template<typename t>
            void clr() {
                this->m<t>::m_map.clear();
            }

            void clrAll() {
                this->_clr<_types...>();
            }

        };

        typedef _PropertyMap<bool,int,float,std::string> PropertyMap;
    }
}
#endif //PROPERTYMAP_H
