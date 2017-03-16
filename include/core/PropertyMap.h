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
        protected:

            void clrChain();

        };

        template<typename _type>
        class _PropertyMapChain<_type> : protected _PropertyMapContainer<_type>{
        protected:

            void clrChain();
        };

        template <typename... _types>
        class _PropertyMap : protected _PropertyMapChain< _types...>  {
        private:
            template<typename _type>
            using m = _PropertyMapContainer<_type>;

        public:
            template<typename t>
            bool has(std::string name);

            template<typename t>
            t& get(std::string name);

            template<typename t>
            t& get(std::string name, t value);

            template<typename t>
            t& set(std::string name, t value);

            template<typename t>
            void del(std::string name);

            template<typename t>
            void clr();

            void clrAll();

        };

        typedef _PropertyMap<bool,int,float,std::string> PropertyMap;
    }
}
#endif //PROPERTYMAP_H
