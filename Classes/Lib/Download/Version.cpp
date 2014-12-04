#include "WkCocos/Download/Version.h"

#include "WkCocos/Utils/ToolBox.h"

#include "WkCocos/Utils/log/logstream.h"

#include <iostream>
#include <sstream>

namespace WkCocos
{
	namespace Download
	{
        const std::pair<unsigned long,std::string> Version::null_convert = {0UL,"0"};

	    Version::Version()
        {
            LogStream::create();
            auto l = LogStream::get();
            if ( l->getAppenders().size() == 0 ) l->addAppender(new LogAppender());
        }

	    Version::Version(std::string vstr, char delim)
        {
            LogStream::create();
            auto l = LogStream::get();
            if ( l->getAppenders().size() == 0 ) l->addAppender(new LogAppender());
            m_version = split(vstr,delim);
        }

        Version::Version(std::vector<unsigned long> version)
        {
            LogStream::create();
            auto l = LogStream::get();
            if ( l->getAppenders().size() == 0 ) l->addAppender(new LogAppender());
            m_version = split(version);
        }

        Version::Version(std::vector<std::string> version)
        {
            LogStream::create();
            auto l = LogStream::get();
            if ( l->getAppenders().size() == 0 ) l->addAppender(new LogAppender());
            m_version = split(version);
        }

        Version::Version(const Version& v)
        :m_version(v.m_version)
        {
        }

        std::vector< std::pair<unsigned long,std::string> > Version::split(const std::string & version_str, char delim)
        {
            std::vector<std::string> sv;
            std::string v = version_str;
            if (v.length() > 0)
            {
                if (v.at(0) == 'v')
                {
                    //removing first 'v'
                    v = v.substr(1);
                }

                size_t current;
                size_t next = -1;
                do
                {
                    current = next + 1;
                    next = v.find_first_of(delim, current);
                    //in case we do not find any delimiter
                    size_t endsub = (std::string::npos == next) ? next : next - current;
                    LOG_DEBUG << v.substr(current, endsub) << std::endl;
                    sv.push_back(v.substr(current, endsub));
                } while (next != std::string::npos);
            }
            return split(sv);
        }

        std::vector< std::pair<unsigned long,std::string> > Version::split(const std::vector<std::string> & version_vec)
        {
            std::vector< std::pair<unsigned long,std::string> > resv;
            for ( std::string nstr : version_vec )
            {
                try
                {
                    unsigned long n = ToolBox::stoul(nstr);
                    resv.push_back(std::make_pair(n,nstr));
                }
                catch (std::out_of_range oor)
                {
                    unsigned long n = failed_conversion_num;
                    resv.push_back(std::make_pair(n,nstr)); // here we keep original str value. it might be needed for accurate comparison later.
                }
            }
            return resv;
        }

	    std::vector< std::pair<unsigned long,std::string> > Version::split(const std::vector<unsigned long> & version_vec)
	    {
            std::vector< std::pair<unsigned long,std::string> > resv;
            for ( unsigned long n : version_vec )
            {
                resv.push_back(std::make_pair(n,ToolBox::to_string(n)));
            }
            return resv;
	    }

        std::pair<unsigned long, std::string>& Version::operator[](int i)
        {
            if ( i >= m_version.size())
            {
                m_version.resize(i+1,null_convert);
            }
            return m_version[i];
        }

        bool Version::operator== (const Version& v) const
        {
            Version mycopy(*this);
            Version vcopy(v);
            bool equals = true;
            for (unsigned int i = 0; equals && i < vcopy.size(); ++i)
            {
                //if we failed conversion to unsigned long ( for very long numbers ) we revert to lexical order
                //TODO : double check and test if it is correct & appropriate behavior
                if ( mycopy[i].first == failed_conversion_num || vcopy[i].first == failed_conversion_num)
                {
                    equals = ( mycopy[i].second == vcopy[i].second );
                }
                else
                {
                    equals = ( mycopy[i].first == vcopy[i].first );
                }
            }

            std::string rel = (equals)? " == " : " != " ;
            LOG_DEBUG << "Version " << mycopy << rel << vcopy << std::endl;

            return equals;
        }

        bool Version::operator< (Version const& v) const
        {
            Version mycopy(*this);
            Version vcopy(v);
            bool thisIsSame = true;
            bool thisIsMore = false;
            //we need to keep checking numbers while we have equality.
            for (unsigned int i = 0; thisIsSame && (!thisIsMore) && i < mycopy.size(); ++i)
            {
                //if we failed conversion to unsigned long ( for very long numbers ) we revert to lexical order
                //TODO : double check and test if it is correct & appropriate behavior
                if ( mycopy[i].first == failed_conversion_num || vcopy[i].first == failed_conversion_num)
                {
                    if ( ! (thisIsSame = (mycopy[i].second == vcopy[i].second) ) )
                    {
                        thisIsMore = ( mycopy[i].second > vcopy[i].second );
                    }
                }

                if ( ! (thisIsSame = (mycopy[i].first == vcopy[i].first) ) )
                {
                    thisIsMore = ( mycopy[i].first > vcopy[i].first );
                }
            }

            std::string rel = (thisIsSame || thisIsMore)? " >= " : " < " ;
            LOG_DEBUG << "Version " << mycopy << rel << vcopy << std::endl;

            return !(thisIsSame || thisIsMore);
        }

        std::string to_string(Version v)
        {
            std::ostringstream oss;
            oss << v;
            return oss.str();
        }

        std::ostream& operator<<(std::ostream& os, const Version& v)
        {
            os << "v";
            for ( auto idx = 0; idx < v.m_version.size(); ++idx )
            {
                if ( idx ) os << Version::default_delim;
                os << v.m_version[idx].second;
            }
            return os;
        }

	} //namespace Download
} //namespace WkCocos
