#pragma once

#include <sol.hpp>
#include <json.hpp>

namespace engine{
    struct Context;
}

/**
* \brief Contains the definitions of the base components
*/
namespace engine::entities::components {
    /**
    * \brief The base of every component.
    *
    * It defines one family of functions to create the component from a lua script and/or a JSON table.
    * It also defines one static function to create an instance of a component.
    *
    * If you create, your own Component (in C++), you must register it before being able to use it. To do so, create a static member of type Component::RegisterComponent in .h file. In the .cpp file, simply initialise the member by passing the name of your component to the constructor of the member.
    */
    class Component {
    public:
        typedef std::shared_ptr<Component> Ptr;

    public:
        Component();
        Component(const Component&) = delete;
        virtual ~Component();

        void setContext(Context &context);

        /**
         * \brief Initialises a component with the given jsonTable.
         * \param jsonTable The JSON table
         */
        virtual void create(const nlohmann::json &jsonTable) = 0;

        virtual void set(const std::string &name, int value) = 0;
        virtual void set(const std::string &name, const std::string& value) = 0;
        virtual void set(const std::string &name, bool value) = 0;

        virtual void set(const std::string &name, sol::nil_t value) = 0;
        virtual void set(const std::string &name, const sol::table& value) = 0;

        virtual void set(const std::string &name, nlohmann::json value) = 0;

        virtual std::pair<int, bool> getInt(const std::string &name) = 0;
        virtual std::pair<sol::object, bool> getObject(const std::string &name) = 0;

        /**
         * \brief Creates an instance of a component based on the given name. If the type is unknown, a generic component is returned.
         * 
         * The component must be registered to be known.
         * \param componentType The type of the component to create
         */
        static Ptr createInstance(Context &context, const std::string &componentType);

        static void luaFunctions(sol::state &lua);

    protected:
        typedef std::map<std::string, std::function<Ptr()>> MapType;

    protected:
        Context& getContext();

        /**
         * \brief Returns the map<string, constructor>
         */
        static std::shared_ptr<MapType> getMapToComponent();

        /**
         * \brief Structure to use when you want to register a component.
         * 
         * Create an instance of this structure in the .h file, then construct it in the .cpp file
         * \tparam The type of the component you want to register
         */
        template <typename T>
        struct RegisterComponent {
            /**
             * \brief Link the name of the component to its constructor
             * \param name The name of the component
             */
            RegisterComponent(const std::string &name) {
                getMapToComponent()->insert(std::make_pair(name, std::make_shared<T>));
            }
        };

    private:
        Context *m_context;
    };
}