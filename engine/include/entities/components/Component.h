#pragma once

#include <sol.hpp>
#include <json.hpp>

/**
* \brief Contains the definitions of the base components
*/
namespace engine::entities::components {
    /**
    * \brief The base of every component.
    *
    * It simply defines a common type (no functions nor variables)
    */
    class Component {
    public:
        Component(const Component&) = delete;
        virtual ~Component() {}
    };
}