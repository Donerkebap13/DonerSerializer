# Changelog

## 1.1.0

### Breaking Changes

- ``DonerSerializer.h`` no longer exists. The Serialize/Deserialize functionalities have been separated in two different files, ``DonerSerialize.h`` and ``DonerDeserialize.h``. This way the user can include only the needed functionality. [PR](https://github.com/Donerkebap13/DonerSerializer/pull/4)
- ``ISerializable`` interface is now located in ``ISerializable.h`` instead of ``DonerSerializer.h``. [PR](https://github.com/Donerkebap13/DonerSerializer/pull/4)
- Namespace ``DonerSerialization`` changed to ``DonerSerializer``, to be more consistent with the library name. [PR](https://github.com/Donerkebap13/DonerSerializer/pull/4)
- ``DONER_SERIALIZE_OBJECT_TO_JSON`` macro has dissapeared. Now you should use ``DonerSerializer::CJsonSerializer`` class. [More info](https://github.com/Donerkebap13/DonerSerializer/tree/development#how-to-serialize) | [PR](https://github.com/Donerkebap13/DonerSerializer/pull/4)
- ``DONER_DESERIALIZE_OBJECT_FROM_JSON`` macro has dissapeared. Now you should use ``DonerSerializer::CJsonDeserializer`` class. [More info](https://github.com/Donerkebap13/DonerSerializer/tree/development#how-to-deserialize) | [PR](https://github.com/Donerkebap13/DonerSerializer/pull/4)

### Features
- ``CSerializationResolver`` and ``CDeserializationResolver`` internal implementation has changed to make it possible to add new **thirdparty** types. [More info](https://github.com/Donerkebap13/DonerSerializer/tree/development#how-to-serialize-thirdparty-types) | [PR](https://github.com/Donerkebap13/DonerSerializer/pull/4)

### Fixes
- Some README typos. [PR](https://github.com/Donerkebap13/DonerSerializer/pull/2)