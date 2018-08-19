# Changelog

## 1.1.0

### Breaking Changes

- ``DonerSerializer.h`` no longer exists. The Serialize/Deserialize functionalities have been separated in two different files, ``DonerSerialize.h`` and ``DonerDeserialize.h``. This way the user can include only the needed functionality.
- ``ISerializable`` interface is now located in ``ISerializable.h`` instead of ``DonerSerializer.h``.
- Namespace ``DonerSerialization`` changed to ``DonerSerializer``, to be more consistent with the library name.
- ``DONER_SERIALIZE_OBJECT_TO_JSON`` macro has dissapeared. Now you should use ``DonerSerializer::CJsonSerializer`` class. [More info](https://github.com/Donerkebap13/DonerSerializer/tree/feature/resolver-refactor-to-add-flexibility#how-to-serialize)
- ``DONER_DESERIALIZE_OBJECT_FROM_JSON`` macro has dissapeared. Now you should use ``DonerSerializer::CJsonDeserializer`` class. [More info](https://github.com/Donerkebap13/DonerSerializer/tree/feature/resolver-refactor-to-add-flexibility#how-to-deserialize)

### Features
- ``CSerializationResolver`` and ``CDeserializationResolver`` internal implementation has changed to make it possible to add new **thirdparty** types. You can check **[here](https://github.com/Donerkebap13/DonerSerializer/tree/feature/resolver-refactor-to-add-flexibility#how-to-serialize-thirdparty-types)** to learn how to.