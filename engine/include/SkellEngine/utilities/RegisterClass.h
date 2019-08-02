/**
 * \file RegisterClass.h
 */

#pragma once

#include <typeinfo>

/**
 * \brief Defines some classes and functions that are used by different parts of the engine
 */
namespace engine::utilities {
    /**
     * \brief Allows to register a constructor for a given class under a certain name.
     * 
     * Once the class is registered, an instance can be initialised by giving the name of the target class (and the needed parameters for the constructor).
     * 
     * The class must inherit from BaseClass and the ConstructorArgs must reflect the arguments of the constructor of every registered class.
     * 
     * \note RegisterClass should not be instantied (since it relies on static variables). However, Register should be initialised (statically) for a class to register
     * \see Register documentation for an example
     * 
     * \warning Due to the way templated classes work in C++, if you use the same BaseClass and ConstructorArgs at different places, it will result in only one mapping! In other words, if you need seperate mappings (for example, you do not want to mix Components and Systems), use different BaseClass and/or ConstructorArgs.
     * \tparam BaseClass The base class that every registered class must inherit from
     * \tparam ConstructorArgs The arguments exepected by the constructor (the const, &, and other modifiers must explicitly be written)
     */
    template <typename BaseClass, typename ... ConstructorArgs>
    class RegisterClass {
    public:
        /**
         * \brief Effecively registers the class T under the name "name"
         * 
         * Since we want to register a class without having to create an instance of the class, the instances of Register must be "static":
         * \code
         *  class Example {
         *  public:
         *      Example() {} // The constructor does not have any parameters
         *  private:
         *      static inline RegisterClass<BiggerExample>::Register<Example> = {"example"};      // will register the class "Example" under the name "example" in the mapping created for the base class "BiggerExample" and when the constructors don't have any parameters. Notice the "static"! The "inline" allows to immediately defines the static variable.
         *  };
         * \endcode
         * 
         * \tparam T The type of the class to register
         * \see REGISTER_CLASS for a generic helper macro and REGISTER_COMPONENT, REGISTER_SYSTEM and REGISTER_STATE for specific helper macros
         */
        template <typename T>
        struct Register {
            /**
             * \brief Register the class T under the given name
             */
            Register(const std::string &name) {
                static_assert(std::is_base_of<BaseClass, T>::value, "RegisterClass: 'T' is not a valid class. It must inherit from 'BaseClass'.");

                if (getMapToClasses().find(name) != getMapToClasses().end()) {
                    tmx::Logger::log("RegisterClass: " + name + " is already defined. The value will be overwritten", tmx::Logger::Type::Warning);
                }
                getMapToClasses().emplace(name, std::make_shared<T, ConstructorArgs...>);
            }
        };

        /**
         * \brief Constructs a pointer to an instance of a registered class.
         * 
         * If "name" is registered, constructs an instance of the class associated using the given constructor parameters.
         * 
         * If "name" is not registered, constructs an empty pointer
         * \param name The name of the class to instantiate
         * \param args The parameters to give to the constructor
         * \return A shared_ptr<BaseClass> with the created instance or nullptr
         */
        static std::shared_ptr<BaseClass> construct(const std::string& name, ConstructorArgs... args) {
            auto constructorItr = getMapToClasses().find(name);

            if (constructorItr == getMapToClasses().end()) {
                return nullptr;
            }
            return constructorItr->second(args...);
        }

    private:
        using constructorSignature = std::function<std::shared_ptr<BaseClass>(ConstructorArgs...)>;
        using mapType = std::map<std::string, constructorSignature>;
    
    private:
        static mapType& getMapToClasses() {
            static mapType map;
            return map;
        }
    };
}

/**
 * \brief Effectively concatenates two objects
 */
#define CONCAT_HELPER(A, B)                                                                         \
    A ## B

/**
 * \brief Concatenates two macro objects
 * 
 * It uses CONCAT_HELPER to force the preprocessor to expand the given macros (if needed) before concatenating
 */
#define CONCAT(A, B)                                                                                \
    CONCAT_HELPER(A, B)

/**
 * \brief Registers the class TYPE under the name NAME by using the REGISTER_OBJECT (which must be RegisterClas::Register with the template parameters set)
 * 
 * \warning It must be placed <b>inside</b> of the class definition. For example, do something like:
 * \code
 * class Foo : public Base {
 *      public:
 *          Foo(Context& context) : { ... }
 *          ...
 *          REGISTER_CLASS(RegisterClass<Base>::Register<Foo>, Foo, "foo")
 * };
 * \endcode
 * 
 * \note This macro adds a <b>private</b> member variable. The name of the variable is the concatenation of "registeringVariable" and the line number in the header file using this macro. This allows to register multiple times the same class under different names
 * \warning This macro uses "private: ". Therefore, everything declared after this macro will be marked as private in your class definition.
 */
#define REGISTER_CLASS(REGISTER_OBJECT, TYPE, NAME)                                                 \
    private:                                                                                        \
        static inline const REGISTER_OBJECT<TYPE> CONCAT(registeringVariable, __LINE__) = {NAME};
