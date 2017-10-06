#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.09";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 1;
	static const long BUILD = 105;
	static const long REVISION = 536;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 123;
	#define RC_FILEVERSION 1,1,105,536
	#define RC_FILEVERSION_STRING "1, 1, 105, 536\0"
	static const char FULLVERSION_STRING[] = "1.1.105.536";
	
	//SVN Version
	static const char SVN_REVISION[] = "2";
	static const char SVN_DATE[] = "2012-08-23T15:58:00.699600Z";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 5;
	

}
#endif //VERSION_H
