#ifndef __WKCOCOS_DOWNLOAD_VERSION_H__
#define __WKCOCOS_DOWNLOAD_VERSION_H__

#include <string>
#include <vector>

namespace WkCocos
{
	namespace Download
	{

	    class Version
	    {
	        public:
            //default version is just "v0" which should be equal to v0.0.0 and less than any version
            Version();

	        Version(std::string vstr);

	        Version(std::vector<unsigned long> version);

	        std::vector<unsigned long> split() const;

            bool operator ==(Version const& v) const;
            bool operator!=(Version const& v) const {return !( operator==(v) );}

            bool operator< (Version const& v) const;
            bool operator> (Version const& v)const {return v.operator<(*this);}
            bool operator<=(Version const& v)const {return !(operator>(v));}
            bool operator>=(Version const& v)const {return !(operator<(v));}

            /**
            * @return true if the number at the same index is equal
            */
            bool isSame(int index,Version const& v) const;
            /**
            * @return true if the number at the same index is equal
            */
            bool isLess(int index,Version const& v) const;

            std::string toString() const
            {
                return m_version_str;
            }
            private:
	        std::string m_version_str;
	    };

	} //namespace Download
} //namespace WkCocos

#endif // __WKCOCOS_DOWNLOAD_VERSION_H__
