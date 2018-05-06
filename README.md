# vmt hooking class
A simple virtual method table hooking class
##Usage:
```cpp
#include "applevmt.hpp"

class idk
{
public:
};

apple_vmt class_name;//vmt class instance
idk* target_table;//the vmt we are hooking


bool __stdcall hFunction(float frame, void* cmd)//function hook
{
	//code
	using func = bool(__stdcall*)(float, void*);//function prototype
	return class_name.get_original<func>(5);//getting the original and returning it
}


void init()
{
	class_name.init(target_table);
	class_name.hook_func(hFunction, 5/*func index*/);

void unhook()
{
	class_name.unhook_func(5);
}
```
