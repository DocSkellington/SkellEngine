#pragma once

#include <Thor/Input/Connection.hpp>

#include <sol/sol.hpp>

namespace engine::events {
    /**
     * \brief Allows to remove a callback from the event handler
     * 
     * If the connection object dies (the variable reaches the end of its scope), the actual connection is <b>not</b> removed (the callback will still be called when an appropriate input is triggered).
     */
    class EventConnection {
    public:
        /**
         * \brief Is the connection still active?
         * \return True iff the callback associated to this connection is still registered in the event handler
         */
        bool isConnected() const;

        /**
         * \brief Disconnects the associated callback from the event handler
         */
        void disconnect();

        static void luaFunctions(sol::state &lua);

    protected:
        /**
         * \brief The (hidden) constructor
         * \param connection The thor::Connection to the registered callback
         */
        EventConnection(const thor::Connection &connection);

    private:
        thor::Connection m_connection;
    };
}