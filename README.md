

# DonerReflection
![Doner Serializer](https://i.imgur.com/DOJNofX.png)

A C++14 header-only library to serialize your class data to JSON.
## What is DonerSerializer?
DonerSerializer is a C++14 header-only library that provides you a simple interface to serialize/deserialize your class data in a few lines of code.

Internally, DonerSerializer uses: 
- [DonerReflection](https://github.com/Donerkebap13/DonerReflection)
- [RapidJson](https://github.com/Tencent/rapidjson)
## Supported types
**Built-in types**
- std::int32_t
- std::uint32_t
- std::int64_t
- std::uint64_t
- float
- double
- bool

**Std containers**
- std::string
- std::vector
- std::list
- std::map
- std::unordered_map

**[User-defined Types](#How-to-Serialize-your-custom-classes)**


## Downloading

You can acquire stable releases [here](https://github.com/Donerkebap13/DonerSerializer/releases).

Alternatively, you can check out the current development version with:

```
git clone https://github.com/Donerkebap13/DonerSerializer.git
```
## Contact

You can contact me directly via [email](mailto:donerkebap13@gmail.com).
Also, if you have any suggestion or you find any bug, please don't hesitate to [create a new Issue](https://github.com/Donerkebap13/DonerReflection/issues).
If you decide to start using **DonerSerializer** in your project, I'll be glad to hear about it and post it here in the main page as an example!
## How to use it
DonerReflecion uses **[DonerReflection](https://github.com/Donerkebap13/DonerReflection)** macros to expose your class data.
```c++
namespace Foo
{
	struct Bar
	{
		int m_int;
		float m_float;
		const char* m_char;
	}
}
// IMPORTANT!!
// It is mandatory that this macro calls happen always outside of any namespace
DONER_DEFINE_REFLECTION_DATA(Foo::Bar,
	DONER_ADD_NAMED_VAR_INFO(m_int, "intFoo"),
	DONER_ADD_NAMED_VAR_INFO(m_float, "floatBar"),
	DONER_ADD_NAMED_VAR_INFO(m_float, "charMander")
)
```
As simple as that. Those macros will define a struct containing all the registered members info for that class. As the comment say, **this calls should be done outside of any namespace**. Otherwise it won't work.
Also, if you don't care about how each member is called in the json, you can use the following macro instead:
```c++
DONER_DEFINE_REFLECTION_DATA(Foo::Bar,
	DONER_ADD_VAR_INFO(m_int),
	DONER_ADD_VAR_INFO(m_float),
	DONER_ADD_VAR_INFO(m_float)
)
```
The name in this case will be the same as the variable name.

In order to access protected/private members, you need to use the following macro:
```c++
namespace Foo
{
	class Bar
	{
	DONER_DECLARE_OBJECT_AS_REFLECTABLE(Bar)
	private:
		int m_int;
		float m_float;
		const char* m_char;
	}
}
// IMPORTANT!!
// It is mandatory that this macro calls happen always outside of any namespace
DONER_DEFINE_REFLECTION_DATA(Foo::Bar,
	DONER_ADD_NAMED_VAR_INFO(m_int, "intFoo"),
	DONER_ADD_NAMED_VAR_INFO(m_float, "floatBar"),
	DONER_ADD_NAMED_VAR_INFO(m_float, "charMander")
)
```
By doing this you allow DonerSerializer to access private members information.
### Inheritance
DonerSerializer doesn't support inheritance per se. If you want to serialize class members inherited from its upper class, you need to do as follows:
```c++
class Foo
{
protected:
	int m_int;
}

class Bar : public Foo
{
DONER_DECLARE_OBJECT_AS_REFLECTABLE(Bar)
private:
	float m_float;
}

DONER_DEFINE_REFLECTION_DATA(Bar,
	DONER_ADD_VAR_INFO(m_int),
	DONER_ADD_VAR_INFO(m_float)
)
```
Even if the upper class have some reflection data defined, **this information is not transitive**, so you need to re-declare it for any children class.
## How to Serialize
You just need to use the macro ``DONER_SERIALIZE_OBJECT_TO_JSON``
```c++
CFoo foo;
foo.m_int = 1337;

rapidjson::Document root;
DONER_SERIALIZE_OBJECT_TO_JSON(foo, root)

// This code is to retireve the json string from rapidjson::Document
rapidjson::StringBuffer strbuf;
strbuf.Clear();
rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
root.Accept(writer);

std::string result(strbuf.GetString()); // value is {"m_int": 1337}
```
## How to Deserialize
You just need to load the json and use the macro ``DONER_DESERIALIZE_OBJECT_FROM_JSON``
```c++
rapidjson::Document parser;
rapidjson::Value& jsonValue = parser.Parse("{\"m_int\": 1337}");

CFoo foo;
DONER_DESERIALIZE_OBJECT_FROM_JSON(foo, jsonValue)

// foo.m_int == 1337 
```
## How to Serialize your custom classes
In order to serialize you own classes, you just need to inherit from ``DonerSerialization::ISerializable`` and to define the desired reflection data [mentioned above](#How-to-use-it)
```c++
class Foo : DonerSerialization::ISerializable
{
DONER_DECLARE_OBJECT_AS_REFLECTABLE(Foo)
protected:
	int m_int;
}
DONER_DEFINE_REFLECTION_DATA(Foo,
	DONER_ADD_VAR_INFO(m_int)
)
// ...
class Bar : public Foo
{
DONER_DECLARE_OBJECT_AS_REFLECTABLE(Bar)
private:
	Foo m_foo;
}
DONER_DEFINE_REFLECTION_DATA(Bar,
	DONER_ADD_VAR_INFO(m_foo)
)
```
