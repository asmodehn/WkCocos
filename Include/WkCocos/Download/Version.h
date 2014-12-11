#ifndef __WKCOCOS_DOWNLOAD_VERSION_H__
#define __WKCOCOS_DOWNLOAD_VERSION_H__

#include <string>
#include <vector>
#include <limits>

namespace WkCocos
{
	namespace Download
	{
	    class Version
	    {
	        public:

				static const unsigned long failed_conversion_num;
				static const char default_delim = '.';
				static const std::pair<unsigned long, std::string> null_convert;

				//default version is just "v0" which should be equal to v0.0.0 and less than any version
				Version();

				Version(std::string vstr, char delim = default_delim);

				Version(std::vector<unsigned long> version);

				Version(std::vector<std::string> version);

				Version(const Version& v);

				bool operator ==(Version const& v) const;
				bool operator!=(Version const& v) const {return !( operator==(v) );}

				bool operator< (Version const& v) const;
				bool operator> (Version const& v)const {return v.operator<(*this);}
				bool operator<=(Version const& v)const {return !(operator>(v));}
				bool operator>=(Version const& v)const {return !(operator<(v));}

				/**
				* overloading subscript operator. increase the size of the vector if needed
				* retrieve from integer version or from string version vector depending on the type needed
				*/
				std::pair<unsigned long, std::string>& operator[](size_t i);

				/**
				* return the size of the vectors ( must always be the same )
				*/
				size_t size() const
				{
					return m_version.size();
				}

				/**
				* Utility static function : Split a version string into a vector of unsigned long.
				* we keep original string in case a conversion to long is not possible ( out of range )
				*/
				static std::vector< std::pair<unsigned long,std::string> > split(const std::string & version_str, char delim);
				static std::vector< std::pair<unsigned long,std::string> > split(const std::vector<std::string> & version_vec);
				static std::vector< std::pair<unsigned long,std::string> > split(const std::vector<unsigned long> & version_vec);

				/**
				* to_string is a friend of us
				*/
				friend std::string to_string(Version v);

				/**
				* operator<< is a friend of us
				*/
				friend std::ostream& operator<<(std::ostream& os, const Version& v);

            private:

				std::vector<std::pair<unsigned long, std::string>> m_version;
	    };


        /**
        * to_string operator overloaded for Version
        */
        std::string to_string(Version v);

        /**
        * operator<< to output to std::ostream
        */
        std::ostream& operator<<(std::ostream& os, const Version& v);

	} //namespace Download
} //namespace WkCocos

#endif // __WKCOCOS_DOWNLOAD_VERSION_H__
