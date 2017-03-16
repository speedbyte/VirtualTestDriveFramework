//
// Created by geislerd on 07.02.17.
//

#include <core/PropertyMap.h>

namespace saliency_sandbox {

    namespace core {

        template<typename _type, typename... _type_next>
        void _PropertyMapChain<_type,_type_next...>::clrChain() {
            this->_PropertyMapContainer<_type>::m_map.clear();
            this->_PropertyMapChain<_type_next...>::clrChain();
        }

        template<typename _type>
        void _PropertyMapChain<_type>::clrChain() {
            this->_PropertyMapContainer<_type>::m_map.clear();
        }

        template <typename... _types>
        template <typename t>
        bool _PropertyMap<_types...>::has(std::string name) {
            return this->m<t>::m_map.find(name) != this->m<t>::m_map.end();
        }

        template <typename... _types>
        template <typename t>
        t& _PropertyMap<_types...>::get(std::string name) {
            sserr << sscond(!this->has<t>(name)) << "No property found with name \"" << name << "\n of type \"" << typeid(t).name() << "\"" << ssthrow;

            return this->m<t>::m_map[name];
        }

        template <typename... _types>
        template <typename t>
        t& _PropertyMap<_types...>::get(std::string name, t value) {
            if(this->has<t>(name))
                return this->m<t>::m_map[name];
            else {
                this->m<t>::m_map[name] = value;
                return this->m<t>::m_map[name];
            }

        }

        template <typename... _types>
        template <typename t>
        t& _PropertyMap<_types...>::set(std::string name, t value) {
            this->m<t>::m_map[name] = value;
            return this->m<t>::m_map[name];
        }

        template <typename... _types>
        template <typename t>
        void _PropertyMap<_types...>::del(std::string name) {
            sserr << sscond(!this->has<t>(name)) << "No property found with name \"" << name << "\n of type \"" << typeid(t).name() << "\"" << ssthrow;

            this->m<t>::m_map.erase(name);
        }

        template <typename... _types>
        template <typename t>
        void _PropertyMap<_types...>::clr() {
            this->m<t>::m_map.clear();
        }

        template <typename... _types>
        void _PropertyMap<_types...>::clrAll() {
            this->_PropertyMapChain<_types...>::clrChain();
        }

        template bool _PropertyMap<bool,int,float,std::string>::has<bool>(std::string name);
        template bool _PropertyMap<bool,int,float,std::string>::has<int>(std::string name);
        template bool _PropertyMap<bool,int,float,std::string>::has<float>(std::string name);
        template bool _PropertyMap<bool,int,float,std::string>::has<std::string>(std::string name);

        template bool& _PropertyMap<bool,int,float,std::string>::get<bool>(std::string name);
        template int& _PropertyMap<bool,int,float,std::string>::get<int>(std::string name);
        template float& _PropertyMap<bool,int,float,std::string>::get<float>(std::string name);
        template std::string& _PropertyMap<bool,int,float,std::string>::get<std::string>(std::string name);

        template bool& _PropertyMap<bool,int,float,std::string>::get<bool>(std::string name, bool value);
        template int& _PropertyMap<bool,int,float,std::string>::get<int>(std::string name, int value);
        template float& _PropertyMap<bool,int,float,std::string>::get<float>(std::string name, float value);
        template std::string& _PropertyMap<bool,int,float,std::string>::get<std::string>(std::string name, std::string value);

        template bool& _PropertyMap<bool,int,float,std::string>::set<bool>(std::string name, bool value);
        template int& _PropertyMap<bool,int,float,std::string>::set<int>(std::string name, int value);
        template float& _PropertyMap<bool,int,float,std::string>::set<float>(std::string name, float value);
        template std::string& _PropertyMap<bool,int,float,std::string>::set<std::string>(std::string name, std::string value);

        template void _PropertyMap<bool,int,float,std::string>::del<bool>(std::string name);
        template void _PropertyMap<bool,int,float,std::string>::del<int>(std::string name);
        template void _PropertyMap<bool,int,float,std::string>::del<float>(std::string name);
        template void _PropertyMap<bool,int,float,std::string>::del<std::string>(std::string name);

        template void _PropertyMap<bool,int,float,std::string>::clr<bool>();
        template void _PropertyMap<bool,int,float,std::string>::clr<int>();
        template void _PropertyMap<bool,int,float,std::string>::clr<float>();
        template void _PropertyMap<bool,int,float,std::string>::clr<std::string>();

        template class _PropertyMap<bool,int,float,std::string>;
    }
}
