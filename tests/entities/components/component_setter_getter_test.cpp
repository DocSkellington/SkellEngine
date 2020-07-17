#include <catch.hpp>

#include "SkellEngine/entities/components/Component.hpp"
#include "SkellEngine/Context.hpp"

using namespace engine;
using namespace entities;
using namespace components;

class TestComponent : public Component {
public:
    TestComponent(states::StateContext &context) :
        Component(context),
        m_int(0),
        m_long(0),
        m_float(0.f),
        m_double(0.),
        m_bool(false),
        m_string("") {
        registerMember<int>("int", &m_int);
        registerMember<long>("long", &m_long);
        registerMember<float>("float", &m_float);
        registerMember<double>("double", &m_double);
        registerMember<bool>("bool", &m_bool);
        registerMember<std::string>("string", &m_string);
        registerMember<nlohmann::json>("json", &m_json);
    }

    TestComponent(const TestComponent&) = delete;
    virtual ~TestComponent() {}

    virtual void create(const nlohmann::json &jsonTable) {}

    int m_int;
    long m_long;
    float m_float;
    double m_double;
    bool m_bool;
    std::string m_string;
    nlohmann::json m_json;
};

SCENARIO("Test of setters", "[entities][components]") {
    Context context("@CMAKE_CURRENT_SOURCE_DIR@/media", false);
    states::StateContext stateContext(context, "falseState");
    TestComponent compo(stateContext);

    GIVEN("An integer variable") {
        REQUIRE(compo.m_int == 0);
        WHEN("We give an int") {
            compo.set("int", 5);
            REQUIRE(compo.m_int == 5);
        }
        WHEN("We give a long") {
            compo.set("int", 18L);
            REQUIRE(compo.m_int == 0);
        }
        WHEN("We give a float") {
            compo.set("int", 4.5f);
            REQUIRE(compo.m_int == 0);
        }
        WHEN("We give a double") {
            compo.set("int", 4.5);
            REQUIRE(compo.m_int == 0);
        }
        WHEN("We give a bool") {
            compo.set("int", true);
            REQUIRE(compo.m_int == 0);
        }
        WHEN("We give a std::string") {
            compo.set("int", "Are");
            REQUIRE(compo.m_int == 0);
        }
        WHEN("We give a nlohmann::json") {
            compo.set("int", nlohmann::json());
            REQUIRE(compo.m_int == 0);
        }
    }

    GIVEN("A long variable") {
        REQUIRE(compo.m_long == 0);
        WHEN("We give an int") {
            compo.set("long", 5);
            REQUIRE(compo.m_long == 5);
        }
        WHEN("We give a long") {
            compo.set("long", 18L);
            REQUIRE(compo.m_long == 18L);
        }
        WHEN("We give a float") {
            compo.set("long", 4.5f);
            REQUIRE(compo.m_long == 0);
        }
        WHEN("We give a double") {
            compo.set("long", 4.5);
            REQUIRE(compo.m_long == 0);
        }
        WHEN("We give a bool") {
            compo.set("long", true);
            REQUIRE(compo.m_long == 0);
        }
        WHEN("We give a std::string") {
            compo.set("long", "you");
            REQUIRE(compo.m_long == 0);
        }
        WHEN("We give a nlohmann::json") {
            compo.set("long", nlohmann::json());
            REQUIRE(compo.m_long == 0);
        }
    }

    GIVEN("A float variable") {
        REQUIRE(compo.m_float == 0.f);
        WHEN("We give an int") {
            compo.set("float", 5);
            REQUIRE(compo.m_float == Approx(5));
        }
        WHEN("We give a long") {
            compo.set("float", 18L);
            REQUIRE(compo.m_float == 0);
        }
        WHEN("We give a float") {
            compo.set("float", 4.5f);
            REQUIRE(compo.m_float == Approx(4.5f));
        }
        WHEN("We give a double") {
            compo.set("float", 4.5);
            REQUIRE(compo.m_float == 0);
        }
        WHEN("We give a bool") {
            compo.set("float", true);
            REQUIRE(compo.m_float == 0);
        }
        WHEN("We give a std::string") {
            compo.set("float", "really");
            REQUIRE(compo.m_float == 0);
        }
        WHEN("We give a nlohmann::json") {
            compo.set("float", nlohmann::json());
            REQUIRE(compo.m_float == 0);
        }
    }

    GIVEN("A double variable") {
        REQUIRE(compo.m_double == 0.f);
        WHEN("We give an int") {
            compo.set("double", 5);
            REQUIRE(compo.m_double == Approx(5));
        }
        WHEN("We give a long") {
            compo.set("double", 18L);
            REQUIRE(compo.m_double == Approx(18));
        }
        WHEN("We give a float") {
            compo.set("double", 4.5f);
            REQUIRE(compo.m_double == Approx(4.5f));
        }
        WHEN("We give a double") {
            compo.set("double", 4.5);
            REQUIRE(compo.m_double == Approx(4.5));
        }
        WHEN("We give a bool") {
            compo.set("double", true);
            REQUIRE(compo.m_double == 0);
        }
        WHEN("We give a std::string") {
            compo.set("double", "reading");
            REQUIRE(compo.m_double == 0);
        }
        WHEN("We give a nlohmann::json") {
            compo.set("double", nlohmann::json());
            REQUIRE(compo.m_double == 0);
        }
    }

    GIVEN("A boolean variable") {
        REQUIRE_FALSE(compo.m_bool);
        WHEN("We give an int") {
            compo.set("bool", 5);
            REQUIRE_FALSE(compo.m_bool);
        }
        WHEN("We give a long") {
            compo.set("bool", 18L);
            REQUIRE_FALSE(compo.m_bool);
        }
        WHEN("We give a float") {
            compo.set("bool", 4.5f);
            REQUIRE_FALSE(compo.m_bool);
        }
        WHEN("We give a double") {
            compo.set("bool", 4.5);
            REQUIRE_FALSE(compo.m_bool);
        }
        WHEN("We give a bool") {
            compo.set("bool", true);
            REQUIRE(compo.m_bool);
        }
        WHEN("We give a std::string") {
            compo.set("bool", "the");
            REQUIRE_FALSE(compo.m_bool);
        }
        WHEN("We give a nlohmann::json") {
            compo.set("bool", nlohmann::json());
            REQUIRE_FALSE(compo.m_bool);
        }
    }

    GIVEN("A std::string variable") {
        REQUIRE(compo.m_string == "");
        WHEN("We give an int") {
            compo.set("string", 5);
            REQUIRE(compo.m_string == "");
        }
        WHEN("We give a long") {
            compo.set("string", 18L);
            REQUIRE(compo.m_string == "");
        }
        WHEN("We give a float") {
            compo.set("string", 4.5f);
            REQUIRE(compo.m_string == "");
        }
        WHEN("We give a double") {
            compo.set("string", 4.5);
            REQUIRE(compo.m_string == "");
        }
        WHEN("We give a bool") {
            compo.set("string", true);
            REQUIRE(compo.m_string == "");
        }
        WHEN("We give a std::string") {
            compo.set("string", "tests");
            REQUIRE(compo.m_string == "tests");
        }
        WHEN("We give a nlohmann::json") {
            compo.set("string", nlohmann::json());
            REQUIRE(compo.m_string == "");
        }
    }

    GIVEN("A nlohmann::json variable") {
        REQUIRE(compo.m_json == nlohmann::json());
        WHEN("We give an int") {
            compo.set("json", 5);
            REQUIRE(compo.m_json == nlohmann::json());
        }
        WHEN("We give a long") {
            compo.set("json", 18L);
            REQUIRE(compo.m_json == nlohmann::json());
        }
        WHEN("We give a float") {
            compo.set("json", 4.5f);
            REQUIRE(compo.m_json == nlohmann::json());
        }
        WHEN("We give a double") {
            compo.set("json", 4.5);
            REQUIRE(compo.m_json == nlohmann::json());
        }
        WHEN("We give a bool") {
            compo.set("json", true);
            REQUIRE(compo.m_json == nlohmann::json());
        }
        WHEN("We give a std::string") {
            compo.set("json", "?");
            REQUIRE(compo.m_json == nlohmann::json());
        }
        WHEN("We give a nlohmann::json") {
            nlohmann::json json = {
                {"test", 42}
            };
            compo.set("json", json);
            REQUIRE(compo.m_json == json);

            compo.set("json", sol::nil_t());
            REQUIRE(compo.m_json == nlohmann::json());
        }
    }
}

SCENARIO("Test of getters", "[entities][components]") {
    nlohmann::json json = {1,2,3};

    Context context("@CMAKE_CURRENT_SOURCE_DIR@/media", false);
    states::StateContext stateContext(context, "falseState");
    TestComponent compo(stateContext);
    compo.m_int = 4;
    compo.m_float = 3.14f;
    compo.m_bool = true;
    compo.m_string = "Testy";
    compo.m_json = json;

    GIVEN("An integer variable") {
        WHEN("We get the variable as an integer") {
            auto p = compo.get<long>("int");
            REQUIRE(p.second);
            REQUIRE(p.first == 4);
        }
        WHEN("We get the variable as a floating point") {
            auto p = compo.get<double>("int");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == 0.);
        }
        WHEN("We get the variable as a boolean") {
            auto p = compo.get<bool>("int");
            REQUIRE_FALSE(p.second);
            REQUIRE_FALSE(p.first);
        }
        WHEN("We get the variable as a std::string") {
            auto p = compo.get<std::string>("int");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == "");
        }
        WHEN("We get the variable as a JSON") {
            auto p = compo.get<nlohmann::json>("int");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == nlohmann::json());
        }
    }

    GIVEN("A floating point variable") {
        WHEN("We get the variable as an integer") {
            auto p = compo.get<long>("float");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == 0);
        }
        WHEN("We get the variable as a floating point") {
            auto p = compo.get<double>("float");
            REQUIRE(p.second);
            REQUIRE(p.first == Approx(3.14f));
        }
        WHEN("We get the variable as a boolean") {
            auto p = compo.get<bool>("float");
            REQUIRE_FALSE(p.second);
            REQUIRE_FALSE(p.first);
        }
        WHEN("We get the variable as a std::string") {
            auto p = compo.get<std::string>("float");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == "");
        }
        WHEN("We get the variable as a JSON") {
            auto p = compo.get<nlohmann::json>("float");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == nlohmann::json());
        }
    }

    GIVEN("A boolean variable") {
        WHEN("We get the variable as an integer") {
            auto p = compo.get<long>("bool");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == 0);
        }
        WHEN("We get the variable as a floating point") {
            auto p = compo.get<double>("bool");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == 0.f);
        }
        WHEN("We get the variable as a boolean") {
            auto p = compo.get<bool>("bool");
            REQUIRE(p.second);
            REQUIRE(p.first);
        }
        WHEN("We get the variable as a std::string") {
            auto p = compo.get<std::string>("bool");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == "");
        }
        WHEN("We get the variable as a JSON") {
            auto p = compo.get<nlohmann::json>("bool");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == nlohmann::json());
        }
    }

    GIVEN("A std::string variable") {
        WHEN("We get the variable as an integer") {
            auto p = compo.get<long>("string");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == 0);
        }
        WHEN("We get the variable as a floating point") {
            auto p = compo.get<double>("string");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == 0.f);
        }
        WHEN("We get the variable as a boolean") {
            auto p = compo.get<bool>("string");
            REQUIRE_FALSE(p.second);
            REQUIRE_FALSE(p.first);
        }
        WHEN("We get the variable as a std::string") {
            auto p = compo.get<std::string>("string");
            REQUIRE(p.second);
            REQUIRE(p.first == "Testy");
        }
        WHEN("We get the variable as a JSON") {
            auto p = compo.get<nlohmann::json>("string");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == nlohmann::json());
        }
    }

    GIVEN("A JSON variable") {
        WHEN("We get the variable as an integer") {
            auto p = compo.get<long>("string");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == 0);
        }
        WHEN("We get the variable as a floating point") {
            auto p = compo.get<double>("string");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == 0.f);
        }
        WHEN("We get the variable as a boolean") {
            auto p = compo.get<bool>("string");
            REQUIRE_FALSE(p.second);
            REQUIRE_FALSE(p.first);
        }
        WHEN("We get the variable as a std::string") {
            auto p = compo.get<std::string>("json");
            REQUIRE_FALSE(p.second);
            REQUIRE(p.first == "");
        }
        WHEN("We get the variable as a JSON") {
            auto p = compo.get<nlohmann::json>("json");
            REQUIRE(p.second);
            REQUIRE(p.first == json);
        }
    }
}