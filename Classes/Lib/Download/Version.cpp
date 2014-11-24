#include "WkCocos/Download/Version.h"

#include "WkCocos/Utils/ToolBox.h"
#include <iostream>

namespace WkCocos
{
	namespace Download
	{
	    Version::Version(std::string vstr)
        : m_version_str(vstr)
        {
            if (m_version_str[0] != 'v')
            {
                throw std::logic_error("This string is not a version string !");
            }
        }

        Version::Version(std::vector<unsigned long> version)
        : m_version_str("v")
        {
            for (unsigned long num : version)
            {
                m_version_str += WkCocos::ToolBox::to_string(num) + ".";
            }
            m_version_str.pop_back(); // removing trailing '.'
        }

        std::vector<unsigned long> Version::split() const
        {
            std::vector<unsigned long> sv;
            std::string v = m_version_str;
            if (v.length() > 0)
            {
                if (v.at(0) == 'v')
                {
                    //removing first 'v'
                    v = v.substr(1);
                }

                std::string delimiters = ".";
                size_t current;
                size_t next = -1;
                do
                {
                    current = next + 1;
                    next = v.find_first_of(delimiters, current);
                    //in case we do not find any delimiter
                    size_t endsub = (std::string::npos == next) ? next : next - current;
                    //std::cout << v.substr(current, next - current) << std::endl;
                    unsigned long tmp = 0;
                    try
                    {
                        tmp = ToolBox::stoul(v.substr(current, endsub));
                    }
                    catch (std::out_of_range oor)
                    {
                        tmp = ULONG_MAX;
                    }
                    sv.push_back(tmp);
                } while (next != std::string::npos);
            }
            return sv;
        }

        bool Version::operator== (Version const& v) const
        {
            Version vcopy = v;
            std::vector<unsigned long> v1l = split();
            std::vector<unsigned long> v2l = vcopy.split();
            bool equals = true;
            //getting all version to same size by assuming missing one is 0.
            while (v1l.size() < v2l.size())
            {
                v1l.push_back(0);
            }
            while (v1l.size() > v2l.size())
            {
                v2l.push_back(0);
            }

            for (unsigned int i = 0; i < v1l.size(); ++i)
            {
                if (v1l.at(i) != v2l.at(i))
                {
                    equals = false; break;
                }
            }
            return equals;
        }

        bool Version::operator< (Version const& v) const
        {
            Version vcopy = v;
            std::vector<unsigned long> v1l = split();
            std::vector<unsigned long> v2l = vcopy.split();
            bool more = true;
            //getting all version to same size by assuming missing one is 0.
            while (v1l.size() < v2l.size())
            {
                v1l.push_back(0);
            }
            while (v1l.size() > v2l.size())
            {
                v2l.push_back(0);
            }

            for (unsigned int i = 0; i < v1l.size(); ++i)
            {
                if (v1l.at(i) < v2l.at(i))
                {
                    more = false; break;// if left number is less, we don't need to check right hand numbers. this version is less.
                }
            }
#ifdef _DEBUG
            if (more)
            {
                std::cout << "Version " << m_version_str << " >= " << v.m_version_str << std::endl;
            }
            else
            {
                std::cout << "Version " << m_version_str << " < " << v.m_version_str << std::endl;
            }
#endif
            return !more;
        }

        bool Version::isSame(int index, Version const& v) const
        {
            Version vcopy = v;
            std::vector<unsigned long> v1l = split();
            std::vector<unsigned long> v2l = vcopy.split();
            return v1l.at(index) == v2l.at(index);
        }

        bool Version::isLess(int index,Version const& v) const
        {
            Version vcopy = v;
            std::vector<unsigned long> v1l = split();
            std::vector<unsigned long> v2l = vcopy.split();
            return v1l.at(index) <= v2l.at(index);
        }

	} //namespace Download
} //namespace WkCocos
