//
// Created by geislerd on 13.02.17.
//

#ifndef CSVREADER_H
#define CSVREADER_H

#include <core/Node.h>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>

namespace saliency_sandbox {
    namespace io {

        template<char _sep = ' ',typename... _fields>
        class CSVReader;

        template<char _sep, typename  _field, typename... _field_next>
        class CSVField {
        private:
            CSVField<_sep,_field_next...> m_next;
            _field m_value;
        public:
            template<typename... _fields>
            void setOutput(CSVReader<_sep,_fields...>* reader) {
                reader->template output<sizeof...(_fields) - 1 - sizeof...(_field_next)>()->value(&(this->m_value));
                this->m_next.setOutput<_fields...>(reader);
            }
            void clear() {
                this->m_value = _field();
                this->m_next.clear();
            }
            friend std::istream& operator>>(std::istream& in, CSVField<_sep,_field, _field_next...>& csvfield) {
                std::string sval;

                csvfield.clear();

                std::getline(in,sval,_sep);
                if(!boost::conversion::detail::try_lexical_convert(sval, csvfield.m_value))
                    ;//TODO: error handling - may end of file

                /*
                if(sval.empty() || !boost::conversion::detail::try_lexical_convert(sval, csvfield.m_value))
                    csvfield.m_value = boost::lexical_cast<_field>(sval);
                else
                    csvfield.m_value = _field(); //TODO: error handling - may end of file
                 */

                in >> csvfield.m_next;

                return in;
            }
        };

        template<char _sep, typename _field>
        class CSVField<_sep,_field> {
        private:
            _field m_value;
        public:
            template<typename... _fields>
            void setOutput(CSVReader<_sep,_fields...>* reader) {
                reader->template output<sizeof...(_fields) - 1>()->value(&(this->m_value));
            }
            void clear() {
                this->m_value = _field();
            }
            friend std::istream& operator>>(std::istream& in, CSVField<_sep,_field>& csvfield) {
                std::string sval;

                csvfield.clear();

                std::getline(in,sval);
                if(!boost::conversion::detail::try_lexical_convert(sval, csvfield.m_value))
                    ;//TODO: error handling - may end of file

                /*
                std::getline(in,sval);
                if(!sval.empty())
                    csvfield.m_value = boost::lexical_cast<_field>(sval);
                else
                    csvfield.clear(); //TODO: error handling - may end of file
                    */

                return in;
            }
        };

        template<char _sep, typename... _fields>
        class CSVLine {
        private:
            CSVField<_sep,_fields...> m_fields;
        public:
            void setOutput(CSVReader<_sep,_fields...>* reader) {
                this->m_fields.template setOutput<_fields...>(reader);
            }
            CSVLine<_sep,_fields...>& operator<<(const std::string &s) {
                std::stringstream ss(s);
                ss >> this->m_fields;
                return *this;
            }
            void clear() {
                this->m_fields.clear();
            }
            friend std::istream& operator>>(std::istream& in, CSVLine<_sep,_fields...>& csvline) {
                std::string sline;

                std::getline(in,sline);

                csvline << sline;
                return in;
            }
        };

        template<char _sep,typename... _fields>
        class CSVReader : public saliency_sandbox::core::Node::template Input<>::template Output<_fields...> {
        private:
            boost::filesystem::path m_path;
            std::ifstream m_stream;
            CSVLine<_sep,_fields...> m_line;
        public:
            CSVReader() { }

            CSVReader(boost::filesystem::path path) : m_path(path) {
                this->m_line.setOutput(this);
                this->reset();
            }

            void open(boost::filesystem::path path) {
                this->m_path = path;
                this->reset();
            }

            void calc() override {
                this->m_stream.peek(); // recognize eof;

                if(!this->m_stream.eof())
                    this->m_stream >> this->m_line;
                else
                    this->m_line.clear();

                this->m_line.setOutput(this);
                this->eof(this->m_stream.eof());
            }

            void reset() override {
                if(this->m_stream.is_open())
                    this->m_stream.close();
                this->m_stream.open(this->m_path.string());
                if(!this->m_stream.is_open())
                    throw new std::exception(); //TODO: error handling - cannot open file
            }
        };
    }
}

#endif //CSVREADER_H
